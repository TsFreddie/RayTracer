/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"

namespace rt {

// Calculate primary ray
Ray RayTracer::calcPrimRay(Camera* camera, int x, int y) {
    int width = camera->getWidth();
    int height = camera->getHeight();
    double fov = camera->getFov();

    double ratio = width / (double)height;

    Ray ray;
    ray.raytype = PRIMARY;
    ray.origin = Vec3d(0, 0, 0);  // TODO: camera transformation

    double rx = (2 * ((x + 0.5) / (double)width) - 1) * tan(fov) * ratio;
    double ry = (1 - 2 * ((y + 0.5) / (double)height)) * tan(fov);

    ray.direction = Vec3d(rx, ry, -1).normalize();
    ray.invDir = 1.0 / ray.direction;

    return ray;
}

Ray RayTracer::calcShadowRay(Hit hit, Vec3d lightPos) {
    Ray ray;
    ray.raytype = SHADOW;
    ray.direction = (lightPos - hit.point).normalize();
    ray.origin = hit.point + hit.normal * BIAS;
    ray.invDir = 1.0 / ray.direction;

    return ray;
}

Ray RayTracer::calcReflectionRay(Ray inRay, Hit hit) {
    Ray outRay;
    outRay.raytype = SECONDARY;
    outRay.direction = inRay.direction -
                       hit.normal * 2 * inRay.direction.dotProduct(hit.normal);
    outRay.origin = hit.point + hit.normal * BIAS;
    outRay.invDir = 1.0 / outRay.direction;

    return outRay;
}

Ray RayTracer::calcRefractionRay(Ray inRay, Hit hit, bool inside) {
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

        // Shadow sampling
        Sampler* sampler = light->getSampler();
        int nsamples = sampler->count();
        int shadowHits = 0;
        sampler->reset(lightDirection);
        for (int i = 0; i < nsamples; ++i) {
            Vec3d offset = sampler->next();

            Vec3d target = light->getPosition() + offset;
            Ray shadowRay = calcShadowRay(nearHit, target);

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
    double roughness = hitMat->getRoughness(nearHit.uv);
    bool inside = false;
    if (ray.direction.dotProduct(nearHit.normal) > 0) {
        inside = true;
        nearHit.normal = -nearHit.normal;
    }

    // Roughness sampling
    Sampler* sampler = scene->getSampler();
    int nsamples = sampler->count();

    if (metallic > 0 && transmit < 1) {
        Ray truthR = calcReflectionRay(ray, nearHit);
        sampler->reset(truthR.direction);
        if (roughness > 0) {
            for (int i = 0; i < nsamples; ++i) {
                Vec3d offset = sampler->next();
                Vec3d target = truthR.origin + truthR.direction + offset * roughness;
                Ray R;
                R.direction = (target - nearHit.point).normalize();
                R.invDir = 1.0 / R.direction;
                R.origin = truthR.origin;
                R.raytype = SECONDARY;
                reflectionColor =
                    reflectionColor + traceRay(scene, R, nbounces - 1);
            }
            reflectionColor =
                reflectionColor * (1 / (double)nsamples) * metallic;
        } else {
            reflectionColor = traceRay(scene, truthR, nbounces - 1) * metallic;
        }
    }

    if (transmit > 0) {
        Ray truthT = calcRefractionRay(ray, nearHit, inside);
        sampler->reset(truthT.direction);
        if (roughness > 0) {
            for (int i = 0; i < nsamples; ++i) {
                Vec3d offset = sampler->next();
                Vec3d target = truthT.origin + truthT.direction + offset * roughness;
                Ray T;
                T.direction = (target - nearHit.point).normalize();
                T.invDir = 1.0 / T.direction;
                T.origin = truthT.origin;
                T.raytype = SECONDARY;
                refractionColor =
                    refractionColor + traceRay(scene, T, nbounces - 1);
            }
            refractionColor =
                refractionColor * (1 / (double)nsamples);
        } else {
            refractionColor = traceRay(scene, truthT, nbounces - 1);
        }
    }

    color =
        color + reflectionColor * (1 - transmit) + refractionColor * transmit;

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
            if (x == 400 && y == 400) {
                int haha = 0;
            }

            // Camera sampling
            Ray truthRay = calcPrimRay(camera, x, y);
            Vec3d focusPlaneOrigin =
                Vec3d(0, 0, -1) * camera->getFocusDistance();
            Vec3d focusPlaneNormal = Vec3d(0, 0, -1);
            double nd = focusPlaneNormal.dotProduct(truthRay.direction);
            if (abs(nd) < 1e-6) {
                // parallel. not likely to happen
                continue;
            }
            Vec3d vo = focusPlaneOrigin - truthRay.origin;
            double distance = vo.dotProduct(focusPlaneNormal) / nd;

            Vec3d focusPoint = truthRay.origin + truthRay.direction * distance;

            Vec3d sampleColor(0);

            Sampler* sampler = camera->getSampler();
            int nsamples = sampler->count();
            // TODO: camera transformation
            sampler->reset(Vec3d(0, 0, -1));
            for (int s = 0; s < nsamples; ++s) {
                Ray sampleRay;
                sampleRay.origin = sampler->next() + truthRay.origin;
                sampleRay.direction =
                    (focusPoint - sampleRay.origin).normalize();
                sampleRay.invDir = 1.0 / sampleRay.direction;
                sampleRay.raytype = PRIMARY;

                sampleColor =
                    sampleColor + traceRay(scene, sampleRay, nbounces);
            }
            pixelbuffer[y * width + x] = sampleColor * (1 / (double)nsamples);
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
