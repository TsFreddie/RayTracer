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
    virtual Vec3d Shade(LightSource *light, Hit hit) { return Vec3d(1,0,1); };

    double getTransmit() { return transmit; }
    double getRoughness() { return roughness; }
    double getMetallic() { return metallic; }

private:
    double transmit; // transparency
    double roughness; // glossiness
    double metallic; // reflectivity

};


} //namespace rt



#endif /* MATERIAL_H_ */
