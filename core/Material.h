/*
 * Material.h
 *
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "rapidjson/document.h"
using namespace rapidjson;

namespace rt{

class Material{
public:

    Material() {};
    virtual ~Material() {};
    static Material* createMaterial(Value& matSpec);

private:
};


} //namespace rt



#endif /* MATERIAL_H_ */
