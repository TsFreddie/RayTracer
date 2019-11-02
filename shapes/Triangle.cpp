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
    uv0 = Vec2d(0, 0);
    uv1 = Vec2d(0, 1);
    uv2 = Vec2d(1, 0);
}

bool Triangle::intersect(Ray ray, Hit *hit) {
    double distance;

    if (!Triangle::intersect(ray, v0, v1, v2, normal, distance)) return false;
    if (!hit) return true;

    hit->shape = this;
    hit->distance = distance;
    hit->normal = normal;
    hit->point = ray.origin + ray.direction * distance;

    Vec3d v01 = v1 - v0;
    Vec3d v02 = v2 - v0;
    Vec3d vp0 = hit->point - v0;
    Vec3d vp1 = hit->point - v1;
    Vec3d vp2 = hit->point - v2;

    double a = (v01).crossProduct(v02).length();
    double u = (vp1).crossProduct(vp2).length() / a;
    double v = (vp2).crossProduct(vp0).length() / a;
    double w = (vp0).crossProduct(vp1).length() / a;

    hit->uv = u * uv0 + v * uv1 + w * uv2;
    return true;
}

bool Triangle::intersect(Ray ray, Vec3d v0, Vec3d v1, Vec3d v2, Vec3d normal,
                         double &dist) {
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
    if (normal.dotProduct(v01.crossProduct(vp0)) < 0) return false;

    Vec3d vp1 = hitPoint - v1;
    if (normal.dotProduct(v12.crossProduct(vp1)) < 0) return false;

    Vec3d vp2 = hitPoint - v2;
    if (normal.dotProduct(v20.crossProduct(vp2)) < 0) return false;

    dist = distance;
    return true;
}

}  // namespace rt
