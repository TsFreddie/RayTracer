/*
 * Shape.cpp
 *
 */
#include "Shape.h"
#include "shapes/Sphere.h"
#include "shapes/Triangle.h"
#include "shapes/Plane.h"

namespace rt {

Shape* Shape::createShape(Value& shapeSpec) {
    std::string shapeType = shapeSpec["type"].GetString();

    Shape* newShape = NULL;
    if (shapeType.compare("sphere") == 0) {
        auto center = shapeSpec["center"].GetArray();
        double radius = shapeSpec["radius"].GetDouble();

        newShape = new Sphere(Vec3d(center[0].GetDouble(), center[1].GetDouble(),
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

        newShape = new Plane(a,b,c);
    }

    if (shapeType.compare("triangle") == 0) {
        auto vertices = shapeSpec["vertices"].GetArray();
        auto v = vertices[0].GetArray();
        Vec3d a = Vec3d(v[0].GetDouble(), v[1].GetDouble(), v[2].GetDouble());
        v = vertices[1].GetArray();
        Vec3d b = Vec3d(v[0].GetDouble(), v[1].GetDouble(), v[2].GetDouble());
        v = vertices[2].GetArray();
        Vec3d c = Vec3d(v[0].GetDouble(), v[1].GetDouble(), v[2].GetDouble());

        newShape = new Triangle(a,b,c);
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
    if (this->material != NULL) {
        delete this->material;
    }
    this->material = NULL;
}

}  // namespace rt
