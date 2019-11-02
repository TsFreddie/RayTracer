/*
 * TriMesh.cpp
 *
 *
 */
#include "TriMesh.h"
#include "Triangle.h"

namespace rt {

TriMesh::TriMesh(int nvert, int ntri) {
    verts = new Vec3d[nvert];
    uv = new Vec2d[nvert];
    tris = new Vec3i[ntri];
    normals = new Vec3d[ntri];
    this->nvert = nvert;
    this->ntri = ntri;
    cvert = 0;
    ctri = 0;
}

void TriMesh::addVert(Vec3d p, Vec2d uv) {
    if (cvert >= nvert) return;

    verts[cvert] = p;
    this->uv[cvert] = uv;

    ++cvert;
}

void TriMesh::addTri(int a, int b, int c) {
    if (a >= cvert || b >= cvert || c >= cvert) return;
    if (ctri >= ntri) return;

    tris[ctri] = Vec3i(a, b, c);

    Vec3d ab = verts[b] - verts[a];
    Vec3d ac = verts[c] - verts[a];

    normals[ctri] = ab.crossProduct(ac).normalize();

    ++ctri;
}

void TriMesh::addTri(int a, int b, int c, Vec3d n) {
    if (a >= cvert || b >= cvert || c >= cvert) return;
    if (ctri >= ntri) return;

    tris[ctri] = Vec3i(a, b, c);
    normals[ctri] = n.normalize();

    ++ctri;
}

TriMesh::~TriMesh() {
    if (verts) delete verts;
    if (uv) delete uv;
    if (tris) delete tris;
    if (normals) delete normals;

    verts = NULL;
    uv = NULL;
    tris = NULL;
    normals = NULL;
}

bool TriMesh::intersect(Ray ray, Hit *hit) {
    int iNearTri = -1;
    double nearDist = INFINITY;
    for (int t = 0; t < ctri; ++t) {
        auto tri = tris[t];
        auto normal = normals[t];

        double distance;
        Vec3d v0 = verts[tri.x];
        Vec3d v1 = verts[tri.y];
        Vec3d v2 = verts[tri.z];

        if (Triangle::intersect(ray, v0, v1, v2, normal, distance)) {
            if (distance < nearDist) {
                nearDist = distance;
                iNearTri = t;
            }
        }
    }

    if (iNearTri < 0) return false;
    if (!hit) return true;

    auto tri = tris[iNearTri];
    Vec3d v0 = verts[tri.x];
    Vec3d v1 = verts[tri.y];
    Vec3d v2 = verts[tri.z];
    Vec2d uv0 = uv[tri.x];
    Vec2d uv1 = uv[tri.y];
    Vec2d uv2 = uv[tri.z];
    Vec3d normal = normals[iNearTri];

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

    return true;
}

}  // namespace rt
