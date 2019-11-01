/*
 * Sphere.h
 *
 *
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "math/geometry.h"
#include "core/RayHitStructs.h"
#include "core/Shape.h"

namespace rt{

class Sphere:public Shape{

public:

	//
	// Constructors
	//
	Sphere();
	Sphere(Vec3d center, double radius):center(center), radius(radius){};

	bool intersect(Ray ray, Hit *hit);

private:

	Vec3d center;
	double radius;

};

} 




#endif
