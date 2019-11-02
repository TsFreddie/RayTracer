/*
 * Shape.cpp
 *
 */
#include "Shape.h"
#include "shapes/Plane.h"
#include "shapes/Sphere.h"
#include "shapes/TriMesh.h"
#include "shapes/Triangle.h"

namespace rt {

Shape* Shape::createShape(Value& shapeSpec) {
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

        int nvert = vertices.Size() < uvs.Size() ? vertices.Size() : uvs.Size();
        int ntri = tris.Size();

        TriMesh* mesh = new TriMesh(nvert, ntri);
        for (int i = 0; i < nvert; ++i) {
            auto vt = vertices[i].GetArray();
            auto uv = uvs[i].GetArray();
            mesh->addVert(
                Vec3d(vt[0].GetDouble(), vt[1].GetDouble(), vt[2].GetDouble()),
                Vec2d(uv[0].GetDouble(), uv[1].GetDouble()));
        }

        for (int i = 0; i < ntri; ++i) {
            auto tri = tris[i].GetArray();
            mesh->addTri(tri[0].GetInt(), tri[1].GetInt(), tri[2].GetInt());
        }
        newShape = mesh;
    }

    if (shapeType.compare("ply") == 0) {
        // TODO: PLY
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

Shape::~Shape() {
    if (material != NULL) {
        delete material;
    }
    material = NULL;
}

}  // namespace rt
