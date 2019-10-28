/*
 * Sphere.cpp
 *
 *
 */
#include "Sphere.h"


namespace rt{


	/**
	 * Computes whether a ray hit the specific instance of a sphere shape and returns the hit data
	 *
	 * @param ray cast ray to check for intersection with shape
	 *
	 * @return hit struct containing intersection information
	 *
	 */
	bool Sphere::intersect(Ray ray, Hit& hit){

		Hit h;
		bool isHit = false;
		Vec3f d = ray.origin - center;
		Vec3f dl = (ray.direction * d);

		float bHalf = dl.x + dl.y + dl.z;

		float c = powf(d.length(), 2) - powf(radius, 2);

		float delta = pow(bHalf, 2) - c;
		
		if (delta < 0) 
			return false;

		float distance;
		if (delta == 0) {
			distance = -bHalf;
		} else {
			float sqrtD = sqrt(delta);
			if (-bHalf > sqrtD) {
				distance = -bHalf - sqrtD;
			} else {
				distance = -bHalf + sqrtD;
			}
		}

		hit.mat = this->material;
		hit.distance = distance;
		hit.point = ray.origin + ray.direction * distance;
		return true;
	}
} //namespace rt


