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
	bool Sphere::intersect(Ray ray, Hit *hit){

		// Vec3d d = ray.origin - center;
		// Vec3d dl = (ray.direction * d);

		// double bHalf = dl.x + dl.y + dl.z;

		// double c = pow(d.length(), 2) - pow(radius, 2);

		// double delta = pow(bHalf, 2) - c;
		
		// if (delta < 0) 
		// 	return false;

		// if (hit == NULL) {
		// 	return true;
		// }

		// double distance;
		// if (delta == 0) {
		// 	distance = -bHalf;
		// } else {
		// 	double sqrtD = sqrt(delta);
		// 	if (-bHalf > sqrtD) {
		// 		distance = -bHalf - sqrtD;
		// 	} else {
		// 		distance = -bHalf + sqrtD;
		// 	}
		// }

		Vec3d l = center - ray.origin;
		double tca = l.dotProduct(ray.direction); // Closest approach
		if (tca < 0) return false; // Ray intersection behind ray origin
		double d2 = l.dotProduct(l) - tca*tca;
		if (d2 > radius * radius) return false; // Ray doesn't intersect
		if (!hit) return true;
		double thc = sqrt(radius * radius - d2); // Closest approach to surface of sphere
		double t0 = tca - thc;
		double t1 = tca + thc;


		// // TODO: FIX THIS!!!!!!!!!!!!!!!!!
		// Vec3d o = ray.origin;
		// Vec3d d = ray.direction;
		// Vec3d oc = o - center;
		// double b = 2 * oc.dotProduct(d);
		// double c = oc.dotProduct(oc) - radius * radius;
		// double disc = b*b - 4*c;
		// if(disc < 0) return false;

		// if (!hit) return true;

		// disc = sqrt(disc);
		// double t0 = -b - disc;
		// double t1 = -b + disc;

		hit->distance = ((t0 < t1) ? t0 : t1);

		// hit->distance = distance;
		hit->shape = this;
		hit->point = ray.origin + (ray.direction * hit->distance);
		hit->normal = ((hit->point - center) * (1/radius));
		return true;
	}

} 


