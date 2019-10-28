/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"


namespace rt{


// Calculate primary ray
Ray RayTracer::primRay(Camera* camera, int x, int y) {
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
			Ray pr = primRay(camera, x, y);
			
			Hit minHit;
			minHit.distance = INFINITY;
			minHit.mat = NULL;

			for (auto it = scene->itShapeBegin(); it != scene->itShapeEnd(); ++it) {
				auto shape = *it;
				Hit hit;

				if (shape->intersect(pr, hit) && hit.distance < minHit.distance) {
					minHit = hit;
				}
			}

			if (minHit.mat != NULL) {
				pixelbuffer[y*width+x] = minHit.mat->Shade(scene, minHit);
			}
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


