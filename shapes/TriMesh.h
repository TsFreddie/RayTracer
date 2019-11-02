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
    TriMesh(int ntris);

    void addTri(Vec3d a, Vec3d b, Vec3d c);
    void addTri(Vec3d a, Vec3d b, Vec3d c, Vec2d uva, Vec2d uvb, Vec2d uvc);
    bool intersect(Ray ray, Hit *hit);

    ~TriMesh();

   private:
    Vec3d *verts;
    Vec3d *normals;
    Vec2d *uv;
};

}  // namespace rt

#endif
