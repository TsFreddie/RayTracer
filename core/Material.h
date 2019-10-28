/*
 * Material.h
 *
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "math/geometry.h"

#include "rapidjson/document.h"
using namespace rapidjson;

namespace rt{

class Material{
public:

    Material() {};
    virtual ~Material() {};
    static Material* createMaterial(Value& matSpec);
    virtual Vec3f Shade() { return Vec3f(1,0,1); };

private:
};


} //namespace rt



#endif /* MATERIAL_H_ */
