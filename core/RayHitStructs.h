/*
 * RayHitStructs.h
 */

#ifndef CORE_RAYHITSTRUCTS_H_
#define CORE_RAYHITSTRUCTS_H_

#include "math/geometry.h"

namespace rt {

/*
 * Ray structure definition
 */
enum RayType { PRIMARY, SECONDARY, SHADOW };

typedef struct _ray {
    RayType raytype;
    Vec3d origin;
    Vec3d direction;
} Ray;

class Shape;
typedef struct _hit {
    Vec3d point;
    Vec3d normal;
    Vec2d uv;
    double distance;
    Shape* shape;
} Hit;

}  // namespace rt

#endif
