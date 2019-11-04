/*
 * Triangle.h
 *
 *
 */

#ifndef TRIMESH_H_
#define TRIMESH_H_

#include <vector>

#include "core/RayHitStructs.h"
#include "core/Shape.h"
#include "math/geometry.h"
#include "shapes/BVH.h"

namespace rt {

class TriMesh : public Shape {
   public:
    //
    // Constructors
    //
    TriMesh(int nverts, int ntris);

    void addVert(Vec3d p, Vec2d uv);
    void addTri(int a, int b, int c);
    void addTri(int a, int b, int c, Vec3d n);
    double intersect(Ray ray, Hit *hit);
    void optimize();

    ~TriMesh();

   private:
    int cverts;
    int ctris;
    int nverts;
    int ntris;
    Vec3d *verts;
    Vec2d *uv;

    // Specialized tri class only for trimesh
    class Tri : public Shape {
       public:
        Tri(int a, int b, int c, Vec3d *verts) : a(a), b(b), c(c) {
            v0 = verts[a];
            v1 = verts[b];
            v2 = verts[c];
            extendBound(v0);
            extendBound(v1);
            extendBound(v2);

            Vec3d ab = v1 - v0;
            Vec3d ac = v2 - v0;

            normal = ab.crossProduct(ac).normalize();
        }
        Tri(int a, int b, int c, Vec3d n, Vec3d *verts)
            : a(a), b(b), c(c), normal(n) {
            v0 = verts[a];
            v1 = verts[b];
            v2 = verts[c];
            extendBound(v0);
            extendBound(v1);
            extendBound(v2);
        }

        double intersect(Ray ray, Hit *hit);

        Vec3i getTri() { return Vec3i(a, b, c); }
        Vec3d getNormal() { return normal; }

       private:
        int a, b, c;
        Vec3d v0, v1, v2;
        Vec3d normal;
    };
    BVH *bvh;
    std::vector<Shape*> tris;
};

}  // namespace rt

#endif
