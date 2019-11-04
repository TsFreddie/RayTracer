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

    virtual double getTransmit(Vec2d uv);
    virtual double getRoughness(Vec2d uv);
    virtual double getMetallic(Vec2d uv);
    virtual double getAmbient();
    virtual Vec3d getDiffuse(Vec2d uv);
    virtual double getRefractiveIndex(bool inverse);
    virtual void setRefractiveIndex(double index);

   private:
    PPMTexture *transmit;   // transparency
    PPMTexture *roughness;  // glossiness
    PPMTexture *metallic;
    PPMTexture *diffuse;
    double refractiveIndex;
    double invRIndex;
    Vec3d diffuseColor;
};

}  // namespace rt

#endif
