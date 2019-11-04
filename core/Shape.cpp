/*
 * Shape.cpp
 *
 */
#include "Shape.h"
#include "parsers/PLYReader.h"
#include "shapes/Plane.h"
#include "shapes/Sphere.h"
#include "shapes/TriMesh.h"
#include "shapes/Triangle.h"

namespace rt {

Shape::Shape() {
    bound.min = Vec3d(INFINITY, INFINITY, INFINITY);
    double negInf = -1 * INFINITY;
    bound.max = Vec3d(negInf, negInf, negInf);
    material = NULL;
}

Shape* Shape::createShape(Value& shapeSpec, bool useBVH) {
    std::string shapeType = shapeSpec["type"].GetString();

    Shape* newShape = NULL;
    if (shapeType.compare("sphere") == 0) {
        auto center = shapeSpec["center"].GetArray();
        double radius = shapeSpec["radius"].GetDouble();

        newShape =
            new Sphere(Vec3d(center[0].GetDouble(), center[1].GetDouble(),
                             center[2].GetDouble()),
                       radius);
    }

    if (shapeType.compare("plane") == 0) {
        auto vertices = shapeSpec["vertices"].GetArray();
        auto v = vertices[0].GetArray();
        Vec3d a = Vec3d(v[0].GetDouble(), v[1].GetDouble(), v[2].GetDouble());
        v = vertices[1].GetArray();
        Vec3d b = Vec3d(v[0].GetDouble(), v[1].GetDouble(), v[2].GetDouble());
        v = vertices[2].GetArray();
        Vec3d c = Vec3d(v[0].GetDouble(), v[1].GetDouble(), v[2].GetDouble());

        newShape = new Plane(a, b, c);
    }

    if (shapeType.compare("triangle") == 0) {
        auto vertices = shapeSpec["vertices"].GetArray();
        auto v = vertices[0].GetArray();
        Vec3d a = Vec3d(v[0].GetDouble(), v[1].GetDouble(), v[2].GetDouble());
        v = vertices[1].GetArray();
        Vec3d b = Vec3d(v[0].GetDouble(), v[1].GetDouble(), v[2].GetDouble());
        v = vertices[2].GetArray();
        Vec3d c = Vec3d(v[0].GetDouble(), v[1].GetDouble(), v[2].GetDouble());

        newShape = new Triangle(a, b, c);
    }

    if (shapeType.compare("trimesh") == 0) {
        auto vertices = shapeSpec["vertices"].GetArray();
        auto uvs = shapeSpec["uv"].GetArray();
        auto tris = shapeSpec["tris"].GetArray();

        int nverts = vertices.Size() < uvs.Size() ? vertices.Size() : uvs.Size();
        int ntris = tris.Size();

        TriMesh* mesh = new TriMesh(nverts, ntris);
        for (int i = 0; i < nverts; ++i) {
            auto vt = vertices[i].GetArray();
            auto uv = uvs[i].GetArray();
            mesh->addVert(
                Vec3d(vt[0].GetDouble(), vt[1].GetDouble(), vt[2].GetDouble()),
                Vec2d(uv[0].GetDouble(), uv[1].GetDouble()));
        }

        for (int i = 0; i < ntris; ++i) {
            auto tri = tris[i].GetArray();
            mesh->addTri(tri[0].GetInt(), tri[1].GetInt(), tri[2].GetInt());
        }
        if (useBVH) mesh->optimize();
        newShape = mesh;
    }

    if (shapeType.compare("ply") == 0) {
        std::string filename = shapeSpec["file"].GetString();
        PLYReader ply(filename.c_str());

        int nverts = ply.nVert();
        int ntris = ply.nTri();

        if (nverts < 0 || ntris < 0) return NULL;

        Vec3d* vnormals = new Vec3d[nverts];
        TriMesh* mesh = new TriMesh(nverts, ntris);

        for (int i = 0; i < nverts; ++i) {
            Vec3d vert, normal;
            Vec2d uv;

            ply.ReadVert(vert, normal, uv);
            mesh->addVert(vert, uv);
            vnormals[i] = normal;
        }

        for (int i = 0; i < ntris; ++i) {
            Vec3i tri;
            if (!ply.ReadTri(tri)) continue;
            if (tri.x >= nverts) continue;
            mesh->addTri(tri.x, tri.y, tri.z, vnormals[tri.x]);
        }
        if (useBVH) mesh->optimize();
        newShape = mesh;
        delete vnormals;
    }

    if (newShape) {
        // Assign material
        if (shapeSpec.HasMember("material")) {
            newShape->material =
                Material::createMaterial(shapeSpec["material"]);
        } else {
            // Default material
            newShape->material = new Material();
        }
    }
    return newShape;
}

void Shape::extendBound(Vec3d point) {
    if (point.x < bound.min.x) bound.min.x = point.x;
    if (point.y < bound.min.y) bound.min.y = point.y;
    if (point.z < bound.min.z) bound.min.z = point.z;
    if (point.x > bound.max.x) bound.max.x = point.x;
    if (point.y > bound.max.y) bound.max.y = point.y;
    if (point.z > bound.max.z) bound.max.z = point.z;
}

void Shape::mergeBound(Bound bound) {
    if (bound.min.x < this->bound.min.x) this->bound.min.x = bound.min.x;
    if (bound.min.y < this->bound.min.y) this->bound.min.y = bound.min.y;
    if (bound.min.z < this->bound.min.z) this->bound.min.z = bound.min.z;
    if (bound.max.x > this->bound.max.x) this->bound.max.x = bound.max.x;
    if (bound.max.y > this->bound.max.y) this->bound.max.y = bound.max.y;
    if (bound.max.z > this->bound.max.z) this->bound.max.z = bound.max.z;
}

bool Shape::hasVolume() {
    if (bound.min.x >= bound.max.x) return false;
    if (bound.min.y >= bound.max.y) return false;
    if (bound.min.z >= bound.max.z) return false;
    return true;
}

Shape::~Shape() {
    if (material != NULL) {
        delete material;
    }
    material = NULL;
}

}  // namespace rt
