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
    Plane(Vec3d a, Vec3d b, Vec3d c);

    bool intersect(Ray ray, Hit *hit);

   private:
    Vec3d v0, v1, v2, v3;
    Vec2d uv0, uv1, uv2, uv3;
    Vec3d normal;
};

}  // namespace rt

#endif
