/*
 * Triangle.h
 *
 *
 */

#ifndef TRIMESH_H_
#define TRIMESH_H_

#include "core/RayHitStructs.h"
#include "core/Shape.h"
#include "math/geometry.h"

namespace rt {

class TriMesh : public Shape {
   public:
    //
    // Constructors
    //
    TriMesh(int nvert, int ntri);
    
    void addVert(Vec3d p, Vec2d uv);
    void addTri(int a, int b, int c);
    void addTri(int a, int b, int c, Vec3d n);
    bool intersect(Ray ray, Hit *hit);

    ~TriMesh();

   private:
    int cvert;
    int ctri;
    int nvert;
    int ntri;
    Vec3d *verts;
    Vec3d *normals;
    Vec2d *uv;
    Vec3i *tris;
};

}  // namespace rt

#endif
