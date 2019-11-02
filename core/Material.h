/*
 * Material.h
 *
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "core/RayHitStructs.h"
#include "math/geometry.h"

#include "rapidjson/document.h"
using namespace rapidjson;

namespace rt {

class LightSource;
class Material {
   public:
    Material();
    virtual ~Material(){};
    static Material* createMaterial(Value& matSpec);
    virtual Vec3d Shade(LightSource* light, Hit hit, Vec3d view) {
        return diffuse;
    };

    double getTransmit() { return transmit; }
    double getRoughness() { return roughness; }
    double getMetallic() { return metallic; }
    Vec3d getDiffuse() { return diffuse; }

   private:
    double transmit;   // transparency
    double roughness;  // glossiness
    double metallic;   // reflectivity
    Vec3d diffuse;
};

}  // namespace rt

#endif
