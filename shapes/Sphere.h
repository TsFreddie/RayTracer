/*
 * Sphere.h
 *
 *
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "core/RayHitStructs.h"
#include "core/Shape.h"
#include "math/geometry.h"

namespace rt {

class Sphere : public Shape {
   public:
    //
    // Constructors
    //
    Sphere(Vec3d center, double radius);

    double intersect(Ray ray, Hit *hit);

   private:
    Vec3d center;
    double radius;
};

}  // namespace rt

#endif
