/*
 * Shape.h
 *
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "core/Material.h"
#include "core/RayHitStructs.h"

#include "rapidjson/document.h"
using namespace rapidjson;

namespace rt {

class Shape {
   public:
    //
    // Constructors
    //
    Shape(){};

    //
    // Destructor (must be overriden in subclass)
    //
    virtual ~Shape();

    //
    // Shape abstract methods (to be implemented by subclasses)
    //
    virtual bool intersect(Ray, Hit*) = 0;

    static Shape* createShape(Value& shapeSpec);
    Material* getMaterial() { return material; }

   protected:
    Material* material;
};

}  // namespace rt

#endif
