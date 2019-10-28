/*
 * Shape.cpp
 *
 */
#include "Shape.h"
#include "shapes/Sphere.h"



namespace rt{

    Shape* Shape::createShape(Value& shapeSpec) {
        std::string shapeType = shapeSpec["type"].GetString();

        Shape* newShape = NULL;
        if (shapeType.compare("sphere") == 0) {
            if (!shapeSpec.HasMember("center") || !shapeSpec["center"].IsArray() ||
                shapeSpec["center"].Size() < 3 ||
                !shapeSpec.HasMember("radius") || !shapeSpec["radius"].IsFloat()) {

                return NULL;
            }
            auto center = shapeSpec["center"].GetArray();
            float radius = shapeSpec["radius"].GetFloat();
            newShape = new Sphere(Vec3f(center[0].GetFloat(), center[1].GetFloat(), center[2].GetFloat()), radius);
        }

        if (newShape) {
            // Assign material
            if (shapeSpec.HasMember("material")) {
                newShape->material = Material::createMaterial(shapeSpec["material"]);
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

} //namespace rt

