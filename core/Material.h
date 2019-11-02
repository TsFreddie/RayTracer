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
class PPMTexture;
class Material {
   public:
    Material();
    virtual ~Material();
    static Material* createMaterial(Value& matSpec);
    virtual Vec3d Shade(LightSource* light, Hit hit, Vec3d view) {
        return diffuseColor;
    };

    double getTransmit(double u, double v);
    double getRoughness(double u, double v);
    double getMetallic(double u, double v);
    Vec3d getDiffuse(double u, double v);

   private:
    PPMTexture *transmit;   // transparency
    PPMTexture *roughness;  // glossiness
    PPMTexture *metallic;   // reflectivity
    PPMTexture *diffuse;
    Vec3d diffuseColor;
};

}  // namespace rt

#endif
