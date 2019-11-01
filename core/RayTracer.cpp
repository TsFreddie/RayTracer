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
	double fov = camera->getFov();

	double ratio = width / (double)height;

	Ray ray;
	ray.raytype = PRIMARY;
	ray.origin = Vec3d(0,0,0); // TODO: give camera position & look at

	double rx = (2*((x + 0.5) / (double) width)-1) * tan(fov) * ratio;
	double ry = (1-2*((y + 0.5) / (double) height)) * tan(fov);

	ray.direction = Vec3d(rx, ry, 1).normalize();

	return ray;
}

Ray RayTracer::createShadowRay(Hit hit, Vec3d lightPos) {
	Ray ray;
	ray.raytype = SHADOW;
	ray.direction = (lightPos - hit.point).normalize();
	ray.origin = hit.point;

	return ray;
}

Vec3d RayTracer::traceRay(Scene *scene, Ray ray, int nbounces) {
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

	// No Hit
	if (minHit.shape == NULL) {
		if (ray.raytype == PRIMARY) {
			return scene->getBackgroundColor();
		} else {
			return Vec3d(0,0,0);
		}
	}

	// TODO: add ambient
	Vec3d color(0,0,0);

	// Lights
	for(auto it = scene->itLightBegin(); it != scene->itLightEnd(); ++it) {
		auto light = *it;

		bool inShadow = false;
		Ray shadowRay;
		shadowRay.direction = (light->getPosition() - minHit.point).normalize();
		shadowRay.origin = minHit.point;

		for (auto it2 = scene->itShapeBegin(); it2 != scene->itShapeEnd(); ++it2) {
			auto shape = *it2;
			if (shape->intersect(shadowRay, NULL)) {
				inShadow = true;
				break;
			}
		}

		if (!inShadow) {
			color = color + minHit.shape->getMaterial()->Shade(light, minHit);
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
Vec3d* RayTracer::render(Camera* camera, Scene* scene, int nbounces){

	int width = camera->getWidth();
	int height = camera->getHeight();
	Vec3d* pixelbuffer = new Vec3d[width * height];

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Ray pr = createPrimRay(camera, x, y);
			pixelbuffer[y*width+x] = traceRay(scene, pr, nbounces);
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
Vec3d* RayTracer::tonemap(Vec3d* pixelbuffer, int width, int height){

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Vec3d color = pixelbuffer[y*width+x] * 255;
			if (color.x > 255) color.x = 255;
			if (color.x < 0) color.x = 0;
			if (color.y > 255) color.y = 255;
			if (color.y < 0) color.y = 0;
			if (color.z > 255) color.z = 255;
			if (color.z < 0) color.z = 0;
			pixelbuffer[y*width+x] = color;
		}
	}
	return pixelbuffer;

}





} //namespace rt


