/*
 * Triangle.cpp
 *
 *
 */
#include "Triangle.h"

namespace rt {

Triangle::Triangle(Vec3d a, Vec3d b, Vec3d c) : v0(a), v1(b), v2(c) {
    Vec3d v01 = v1 - v0;
    Vec3d v02 = v2 - v0;
    normal = v01.crossProduct(v02).normalize();
}

bool Triangle::intersect(Ray ray, Hit *hit) {
    double nd = normal.dotProduct(ray.direction);
    if (abs(nd) < 1e-6) {
        return false;
    }

    Vec3d vo = v0 - ray.origin;
    double distance = vo.dotProduct(normal) / nd;

    if (distance < 0) return false;

    Vec3d hitPoint = ray.origin + ray.direction * distance;

    Vec3d v01 = v1 - v0;
    Vec3d v12 = v2 - v1;
    Vec3d v20 = v0 - v2;

    Vec3d vp0 = hitPoint - v0;
    if (normal.dotProduct(v01.crossProduct(vp0)) < 0)
        return false;

    Vec3d vp1 = hitPoint - v1;
    if (normal.dotProduct(v12.crossProduct(vp1)) < 0)
        return false;

    Vec3d vp2 = hitPoint - v2;
    if (normal.dotProduct(v20.crossProduct(vp2)) < 0)
        return false;

    if (!hit) return true;

    hit->shape = this;
    hit->distance = distance;
    hit->normal = normal;
    hit->point = ray.origin + ray.direction * distance;

    return true;
}

}  // namespace rt
