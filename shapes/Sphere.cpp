/*
 * Sphere.cpp
 *
 *
 */
#include "Sphere.h"

namespace rt {

Sphere::Sphere(Vec3d center, double radius) : center(center), radius(radius) {
    extendBound(center + Vec3d(1, 1, 1) * radius);
    extendBound(center + Vec3d(-1, -1, -1) * radius);
}

/**
 * Computes whether a ray hit the specific instance of a sphere shape and
 * returns the hit data
 *
 * @param ray cast ray to check for intersection with shape
 *
 * @return hit struct containing intersection information
 *
 */
double Sphere::intersect(Ray ray, Hit *hit) {
    Vec3d l = center - ray.origin;
    double tca = l.dotProduct(ray.direction);
    if (tca < 0) return -1;
    double d2 = l.dotProduct(l) - tca * tca;
    if (d2 > radius * radius) return -1;

    double thc = sqrt(radius * radius - d2);
    double t0 = tca - thc;
    double t1 = tca + thc;

    double distance = ((t0 < t1) ? t0 : t1);

    if (!hit) return distance;

    hit->distance = distance;
    hit->shape = this;
    hit->point = ray.origin + (ray.direction * distance);
    hit->normal = ((hit->point - center) * (1 / radius));

    Vec3d d = -hit->normal;
    hit->uv = Vec2d(0.5 + atan2(d.z, d.x) / (atan(1) * 8),
                    0.5 - asin(d.y) / (atan(1) * 4));
    return distance;
}

}  // namespace rt
