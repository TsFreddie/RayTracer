/*
 * Shape.h
 *
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "core/Material.h"
#include "core/RayHitStructs.h"

#include "rapidjson/document.h"

#include "parsers/PLYReader.h"
using namespace rapidjson;

namespace rt {

class Shape {
   public:
    //
    // Constructors
    //
    Shape();

    //
    // Destructor (must be overriden in subclass)
    //
    virtual ~Shape();

    //
    // Shape abstract methods (to be implemented by subclasses)
    //
    virtual double intersect(Ray, Hit*) = 0;

    static Shape* createShape(Value& shapeSpec);
    Material* getMaterial() { return material; }
    Bound getBound() { return bound; }
    void extendBound(Vec3d point);
    void mergeBound(Bound bound);
    bool hasVolume();
    

   protected:
    Material* material;
    Bound bound;
};

}  // namespace rt

#endif
