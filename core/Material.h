/*
 * Material.h
 *
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "math/geometry.h"
#include "core/RayHitStructs.h"

#include "rapidjson/document.h"
using namespace rapidjson;

namespace rt{

class LightSource;
class Material{
public:

    Material();
    virtual ~Material() {};
    static Material* createMaterial(Value& matSpec);
    virtual Vec3f Shade(LightSource *light, Hit hit) { return Vec3f(1,0,1); };

    float getTransmit() { return transmit; }
    float getRoughness() { return roughness; }
    float getMetallic() { return metallic; }

private:
    float transmit; // transparency
    float roughness; // glossiness
    float metallic; // reflectivity

};


} //namespace rt



#endif /* MATERIAL_H_ */
