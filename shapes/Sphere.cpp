/*
 * Sphere.cpp
 *
 *
 */
#include "Sphere.h"

namespace rt {

/**
 * Computes whether a ray hit the specific instance of a sphere shape and
 * returns the hit data
 *
 * @param ray cast ray to check for intersection with shape
 *
 * @return hit struct containing intersection information
 *
 */
bool Sphere::intersect(Ray ray, Hit *hit) {
    Vec3d l = center - ray.origin;
    double tca = l.dotProduct(ray.direction);  // Closest approach
    if (tca < 0) return false;  // Ray intersection behind ray origin
    double d2 = l.dotProduct(l) - tca * tca;
    if (d2 > radius * radius) return false;  // Ray doesn't intersect
    if (!hit) return true;
    double thc =
        sqrt(radius * radius - d2);  // Closest approach to surface of sphere
    double t0 = tca - thc;
    double t1 = tca + thc;

    hit->distance = ((t0 < t1) ? t0 : t1);
    hit->shape = this;
    hit->point = ray.origin + (ray.direction * hit->distance);
    hit->normal = ((hit->point - center) * (1 / radius));

    Vec3d d = -hit->normal;
    hit->uv = Vec2d(0.5 + atan2(d.z, d.x) / (atan(1) * 8),
                    0.5 - asin(d.y) / (atan(1) * 4));
    return true;
}

}  // namespace rt
