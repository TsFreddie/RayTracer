/*
 * Plane.h
 *
 *
 */

#ifndef PLANE_H_
#define PLANE_H_

#include "core/RayHitStructs.h"
#include "core/Shape.h"
#include "math/geometry.h"

namespace rt {

class Plane : public Shape {
   public:
    //
    // Constructors
    //
    Plane();
    Plane(Vec3d a, Vec3d b, Vec3d c, Vec3d d) {};

    bool intersect(Ray ray, Hit *hit);

   private:
    Vec3d center;
    double radius;
};

}  

#endif
