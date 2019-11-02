/*
 * Plane.cpp
 *
 *
 */
#include "Plane.h"

namespace rt {

Plane::Plane(Vec3d a, Vec3d b, Vec3d c) : v0(a), v1(b), v2(c) {
    Vec3d v01 = v1 - v0;
    Vec3d v02 = v2 - v0;
    v3 = v1 + v02;
    normal = v01.crossProduct(v02).normalize();
    uv0 = Vec2d(0,0);
    uv1 = Vec2d(0,1);
    uv2 = Vec2d(1,0);
    uv3 = Vec2d(1,1);
}

bool Plane::intersect(Ray ray, Hit *hit) {
    double nd = normal.dotProduct(ray.direction);
    if (abs(nd) < 1e-6) {
        return false;
    }

    Vec3d vo = v0 - ray.origin;
    double distance = vo.dotProduct(normal) / nd;

    if (distance < 0) return false;

    Vec3d hitPoint = ray.origin + ray.direction * distance;

    // Check if intersection point is inside triangle
    Vec3d C;

    Vec3d v01 = v1 - v0;
    Vec3d v13 = v3 - v1;
    Vec3d v32 = v2 - v3;
    Vec3d v20 = v0 - v2;

    Vec3d v0p = hitPoint - v0;
    if (normal.dotProduct(v01.crossProduct(v0p)) < 0)
        return false;

    Vec3d v1p = hitPoint - v1;
    if (normal.dotProduct(v13.crossProduct(v1p)) < 0)
        return false;

    Vec3d v3p = hitPoint - v3;
    if (normal.dotProduct(v32.crossProduct(v3p)) < 0)
        return false;

    Vec3d v2p = hitPoint - v2;
    if (normal.dotProduct(v20.crossProduct(v2p)) < 0)
        return false;

    if (!hit) return true;

    hit->shape = this;
    hit->distance = distance;
    hit->normal = normal;
    hit->point = ray.origin + ray.direction * distance;

    double u = v01.dotProduct(v0p) / v01.dotProduct(v01);
    double v = (-v20).dotProduct(v0p) / v20.dotProduct(v20);

    hit->uv = uv0*(1-v)*(1-u)+uv1*(1-v)*u+uv2*v*(1-u)+uv3*u*v;

    return true;
}

}  // namespace rt
