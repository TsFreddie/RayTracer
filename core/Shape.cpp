/*
 * Shape.cpp
 *
 */
#include "Shape.h"



namespace rt{

    Shape* Shape::createShape(Value& shapeSpec) {
        return NULL;
    }

    Shape::~Shape() {
        delete this->material;
        this->material = NULL;
    }

} //namespace rt

