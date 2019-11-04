/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"

namespace rt {

// Calculate primary ray
Ray RayTracer::createPrimRay(Camera* camera, int x, int y) {
    int width = camera->getWidth();
    int height = camera->getHeight();
    double fov = camera->getFov();

    double ratio = width / (double)height;

    Ray ray;
    ray.raytype = PRIMARY;
    ray.origin = Vec3d(0, 0, 0);  // TODO: give camera position & look at

    double rx = (2 * ((x + 0.5) / (double)width) - 1) * tan(fov) * ratio;
    double ry = (1 - 2 * ((y + 0.5) / (double)height)) * tan(fov);

    ray.direction = Vec3d(rx, ry, -1).normalize();
    ray.invDir = 1.0 / ray.direction;

    return ray;
}

Ray RayTracer::createShadowRay(Hit hit, Vec3d lightPos) {
    Ray ray;
    ray.raytype = SHADOW;
    ray.direction = (lightPos - hit.point).normalize();
    ray.origin = hit.point + hit.normal * BIAS;
    ray.invDir = 1.0 / ray.direction;

    return ray;
}

Ray RayTracer::createReflectionRay(Ray inRay, Hit hit) {
    Ray outRay;
    outRay.raytype = SECONDARY;
    outRay.direction = inRay.direction -
                       hit.normal * 2 * inRay.direction.dotProduct(hit.normal);
    outRay.origin = hit.point + hit.normal * BIAS;
    outRay.invDir = 1.0 / outRay.direction;

    return outRay;
}

Ray RayTracer::createRefractionRay(Ray inRay, Hit hit, bool inside) {
    Ray outRay;
    Vec3d normal = hit.normal;
    outRay.raytype = SECONDARY;
    double ratio = hit.shape->getMaterial()->getRefractiveIndex(inside);
    double costheta = -normal.dotProduct(inRay.direction);
    double k = 1 - ratio * ratio * (1 - costheta * costheta);

    outRay.direction =
        (inRay.direction * ratio + normal * (ratio * costheta - sqrt(k)))
            .normalize();
    outRay.origin = hit.point - hit.normal * BIAS;
    outRay.invDir = 1.0 / outRay.direction;

    return outRay;
}

Vec3d RayTracer::traceRay(Scene* scene, Ray ray, int nbounces) {
    Hit nearHit;
    nearHit.distance = INFINITY;
    nearHit.shape = NULL;
    BVH* bvh = scene->getBVH();

    if (bvh != NULL) {
        // Use BVH
        Hit hit;
        if (bvh->intersect(ray, &hit) >= 0) {
            if (hit.distance < nearHit.distance) {
                nearHit = hit;
            }
        }
    } else {
        // Use Iterate
        for (auto it = scene->itShapeBegin(); it != scene->itShapeEnd(); ++it) {
            auto shape = *it;
            Hit hit;
            if (shape->intersect(ray, &hit) >= 0) {
                if (hit.distance < nearHit.distance) {
                    nearHit = hit;
                }
            }
        }
    }

    // No Hit
    if (nearHit.shape == NULL) {
        if (ray.raytype == PRIMARY) {
            return scene->getBackgroundColor();
        } else {
            return Vec3d(0, 0, 0);
        }
    }

    // TODO: camera transformation
    Vec3d view = (Vec3d(0, 0, 0) - nearHit.point).normalize();

    Material* hitMat = nearHit.shape->getMaterial();
    Vec3d color = hitMat->getAmbient() * scene->getAmbientIntensity();

    // Shading
    for (auto it = scene->itLightBegin(); it != scene->itLightEnd(); ++it) {
        auto light = *it;

        Vec3d lightDirection = light->getPosition() - nearHit.point;

        Sampler* sampler = light->getSampler();
        int nsamples = sampler->count();
        int shadowHits = 0;
        sampler->reset(lightDirection);
        for (int i = 0; i < nsamples; ++i) {
            Vec3d offset = sampler->next();

            Vec3d target = light->getPosition() + offset;
            Ray shadowRay = createShadowRay(nearHit, target);

            if (bvh != NULL) {
                if (bvh->intersect(shadowRay, NULL) >= 0) {
                    shadowHits += 1;
                }
            } else {
                for (auto it2 = scene->itShapeBegin();
                     it2 != scene->itShapeEnd(); ++it2) {
                    auto shape = *it2;
                    if (shape->intersect(shadowRay, NULL) >= 0) {
                        shadowHits += 1;
                        break;
                    }
                }
            }
        }

        double shadowAmount = shadowHits / (double)nsamples;

        color =
            color + nearHit.shape->getMaterial()->Shade(light, nearHit, view) *
                        (1 - shadowAmount);
    }

    if (nbounces < 0) {
        return color;
    }

    // If have reflection or refraction, do those
    Vec3d reflectionColor(0);
    Vec3d refractionColor(0);
    double metallic = hitMat->getMetallic(nearHit.uv);
    double transmit = hitMat->getTransmit(nearHit.uv);
    bool inside = false;
    if (ray.direction.dotProduct(nearHit.normal) > 0) {
        inside = true;
        nearHit.normal = -nearHit.normal;
    }

    if (metallic > 0 && transmit < 1) {
        Ray R = createReflectionRay(ray, nearHit);
        reflectionColor =
            traceRay(scene, R, nbounces - 1) * metallic;
    }

    if (transmit > 0) {
        Ray T = createRefractionRay(ray, nearHit, inside);
        refractionColor = traceRay(scene, T, nbounces - 1);
    }

    color = color + reflectionColor * (1 - transmit) +
            refractionColor * transmit;

    return color;
}

/**
 * Performs ray tracing to render a photorealistic scene
 *
 * @param camera the camera viewing the scene
 * @param scene the scene to render, including objects and lightsources
 * @param nbounces the number of bounces to consider for raytracing
 *
 * @return a pixel buffer containing pixel values in linear RGB format
 */
Vec3d* RayTracer::render(Camera* camera, Scene* scene, int nbounces) {
    int width = camera->getWidth();
    int height = camera->getHeight();
    Vec3d* pixelbuffer = new Vec3d[width * height];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Ray pr = createPrimRay(camera, x, y);
            pixelbuffer[y * width + x] = traceRay(scene, pr, nbounces);
        }
    }

    return pixelbuffer;
}

/**
 * Tonemaps the rendered image (conversion of linear RGB values [0-1] to low
 * dynamic range [0-255]
 *
 * @param pixelbuffer the array of size width*height containing linear RGB pixel
 * values
 *
 * @return the tonemapped image
 */
Vec3d* RayTracer::tonemap(Vec3d* pixelbuffer, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Vec3d color = pixelbuffer[y * width + x] * 255;
            if (color.x > 255) color.x = 255;
            if (color.x < 0) color.x = 0;
            if (color.y > 255) color.y = 255;
            if (color.y < 0) color.y = 0;
            if (color.z > 255) color.z = 255;
            if (color.z < 0) color.z = 0;
            pixelbuffer[y * width + x] = color;
        }
    }
    return pixelbuffer;
}

}  // namespace rt
