/*
 * TriMesh.cpp
 *
 *
 */
#include "TriMesh.h"
#include "Triangle.h"

namespace rt {

TriMesh::TriMesh(int nverts, int ntris) {
    verts = new Vec3d[nverts];
    uv = new Vec2d[nverts];
    tris.reserve(ntris);
    this->nverts = nverts;
    this->ntris = ntris;
    cverts = 0;
    ctris = 0;
    bvh = NULL;
}

void TriMesh::addVert(Vec3d p, Vec2d uv) {
    if (cverts >= nverts) return;

    verts[cverts] = p;
    extendBound(p);
    this->uv[cverts] = uv;

    ++cverts;
}

void TriMesh::addTri(int a, int b, int c) {
    if (a >= cverts || b >= cverts || c >= cverts) return;
    if (ctris >= ntris) return;

    tris.push_back(new Tri(a, b, c, verts));

    ++ctris;
}

void TriMesh::addTri(int a, int b, int c, Vec3d n) {
    if (a >= cverts || b >= cverts || c >= cverts) return;
    if (ctris >= ntris) return;

    tris.push_back(new Tri(a, b, c, n, verts));

    ++ctris;
}

void TriMesh::optimize() {
    bvh = new BVH(tris, 0, (int)tris.size());
}

TriMesh::~TriMesh() {
    if (verts) delete verts;
    if (uv) delete uv;

    verts = NULL;
    uv = NULL;

    for (auto it = tris.begin(); it != tris.end(); ++it) {
        delete *it;
    }
}

double TriMesh::Tri::intersect(Ray ray, Hit *hit) {
    double distance;
    if (!Triangle::intersect(ray, v0, v1, v2, normal, distance)) return -1;

    if (hit != NULL) hit->shape = this;
    return distance;
}

double TriMesh::intersect(Ray ray, Hit *hit) {
    Vec3i nearTri;
    Vec3d nearNormal;
    double nearDist = INFINITY;

    if (bvh != NULL) {
        // Use bvh for mesh
        nearDist = bvh->intersect(ray, hit);
        if (hit != NULL && nearDist >= 0) {
            Tri *tri = dynamic_cast<Tri *>(hit->shape);
            nearTri = tri->getTri();
            nearNormal = tri->getNormal();
        }
    } else {
        for (auto it = tris.begin(); it != tris.end(); ++it) {
            Tri *tri = dynamic_cast<Tri *>(*it);
            double distance = tri->intersect(ray, NULL);
            if (distance >= 0 && distance < nearDist) {
                nearDist = distance;
                nearTri = tri->getTri();
                nearNormal = tri->getNormal();
            }
        }
    }

    if (nearDist >= INFINITY || nearDist < 0) return -1;
    if (!hit) return nearDist;

    Vec3d v0 = verts[nearTri.x];
    Vec3d v1 = verts[nearTri.y];
    Vec3d v2 = verts[nearTri.z];
    Vec2d uv0 = uv[nearTri.x];
    Vec2d uv1 = uv[nearTri.y];
    Vec2d uv2 = uv[nearTri.z];
    Vec3d normal = nearNormal;

    hit->shape = this;
    hit->distance = nearDist;
    hit->normal = normal;
    hit->point = ray.origin + ray.direction * hit->distance;

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

    return nearDist;
}

}  // namespace rt
