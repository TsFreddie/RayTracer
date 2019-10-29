/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"


namespace rt{


// Calculate primary ray
Ray RayTracer::createPrimRay(Camera* camera, int x, int y) {
	int width = camera->getWidth();
	int height = camera->getHeight();
	float fov = camera->getFov();

	if (x == 400 && y == 400) {
		int k = 0;
	}
	float ratio = width / (float)height;

	Ray ray;
	ray.raytype = PRIMARY;
	ray.origin = Vec3f(0,0,0); // TODO: give camera position & look at

	float rx = (2*((x + 0.5f) / width)-1) * tan(fov / 2.0f) * ratio;
	float ry = (1-2*((y + 0.5f) / height)) * tan(fov / 2.0f);

	ray.direction = Vec3f(rx, ry, 1).normalize();

	return ray;
}

Ray RayTracer::createShadowRay(Hit hit, Vec3f lightPos) {
	Ray ray;
	ray.raytype = SHADOW;

	ray.direction = (lightPos - hit.point).normalize();
	ray.origin = hit.point;

	return ray;
}

Vec3f RayTracer::trace(Scene *scene, Ray ray, int nbounces) {
	Vec3f color;
	Hit minHit;
	minHit.distance = INFINITY;
	minHit.shape = NULL;
	for (auto it = scene->itShapeBegin(); it != scene->itShapeEnd(); ++it) {
		auto shape = *it;
		Hit hit;
		if (shape->intersect(ray, &hit)) {
			if (hit.distance < minHit.distance) {
				minHit = hit;
			}
		}
	}

	if (minHit.shape == NULL) {
		if (ray.raytype == PRIMARY) {
			return scene->getBackgroundColor();
		} else {
			return Vec3f(0,0,0);
		}
	}

	// Compute illumination with shadows
	for(auto it = scene->itLightBegin(); it != scene->itLightEnd(); ++it) {
		auto light = *it;
		bool inShadow = false;
		Ray shadowRay = createShadowRay(minHit, light->getPosition());

		for (auto it = scene->itShapeBegin(); it != scene->itShapeEnd(); ++it) {
			auto shape = *it;
			if (shape->intersect(shadowRay, NULL)) {
				inShadow = true;
				break;
			}
		}
		if (!inShadow) {
			color = color + minHit.shape->getMaterial()->Shade(scene, minHit);
		}
	}
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
Vec3f* RayTracer::render(Camera* camera, Scene* scene, int nbounces){

	int width = camera->getWidth();
	int height = camera->getHeight();
	Vec3f* pixelbuffer = new Vec3f[width * height];

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Ray pr = createPrimRay(camera, x, y);
			pixelbuffer[y*width+x] = trace(scene, pr, nbounces);
			// Hit minHit;
			// minHit.distance = INFINITY;
			// minHit.shape = NULL;

			// for (auto it = scene->itShapeBegin(); it != scene->itShapeEnd(); ++it) {
			// 	auto shape = *it;
			// 	Hit hit;

			// 	if (shape->intersect(pr, &hit) && hit.distance < minHit.distance) {
			// 		minHit = hit;
			// 	}
			// }

			// if (minHit.shape == NULL) {
			// 	pixelbuffer[y*width+x] = scene->getBackgroundColor();
			// 	continue;
			// }

			// bool inShadow = false;
			// Shape* by;
			// Vec3f intensity(0,0,0);
			// for (auto it = scene->itLightBegin(); it != scene->itLightEnd(); ++it) {
			// 	auto light = *it;
			// 	Ray sr = shadowRay(minHit, light->getPosition());
			// 	for (auto it = scene->itShapeBegin(); it != scene->itShapeEnd(); ++it) {
			// 		auto shape = *it;

			// 		if (minHit.shape == shape) {
			// 			continue;
			// 		}
	
			// 		if (shape->intersect(sr, NULL)) {
			// 			inShadow = true;
			// 			by = shape;
			// 			break;
			// 		}
			// 	}
			// 	if (inShadow) {
			// 		break;
			// 	}

			// 	intensity = intensity + light->getIntensity();
			// }

			// if (inShadow) {
			// 	pixelbuffer[y*width+x] = by->getMaterial()->Shade(scene, minHit);
			// } else {
			// 	pixelbuffer[y*width+x] = minHit.shape->getMaterial()->Shade(scene, minHit) * intensity;
			// }
			

		}
	}





	return pixelbuffer;

}

/**
 * Tonemaps the rendered image (conversion of linear RGB values [0-1] to low dynamic range [0-255]
 *
 * @param pixelbuffer the array of size width*height containing linear RGB pixel values
 *
 * @return the tonemapped image
 */
Vec3f* RayTracer::tonemap(Vec3f* pixelbuffer, int width, int height){

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Vec3f color = pixelbuffer[y*width+x] * 255;
			pixelbuffer[y*width+x] = color;
		}
	}

	return pixelbuffer;

}





} //namespace rt


