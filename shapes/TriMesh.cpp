/*
 * TriMesh.cpp
 *
 *
 */
#include "TriMesh.h"


namespace rt{

TriMesh::TriMesh(int nvert, int nface) {
    
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

    double a = (v01).crossProduct(v20).length();
    double u = (vp1).crossProduct(vp2).length() / a;
    double v = (vp2).crossProduct(vp0).length() / a;
    double w = (vp0).crossProduct(vp1).length() / a;

    hit->uv = u * uv0 + v * uv1 + w * uv2;

    return true;
}



} 


