/*
 * Triangle.h
 *
 *
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "core/RayHitStructs.h"
#include "core/Shape.h"
#include "math/geometry.h"

namespace rt {

class Triangle : public Shape {
   public:
    //
    // Constructors
    //
    Triangle(Vec3d a, Vec3d b, Vec3d c);

    bool intersect(Ray ray, Hit *hit);

   private:
    Vec3d v0, v1, v2;
    Vec2d uv0, uv1, uv2;
    Vec3d normal;
};

}  // namespace rt

#endif
