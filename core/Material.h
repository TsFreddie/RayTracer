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

    double getTransmit(Vec2d uv);
    double getRoughness(Vec2d uv);
    double getMetallic(Vec2d uv);
    Vec3d getDiffuse(Vec2d uv);

   private:
    PPMTexture *transmit;   // transparency
    PPMTexture *roughness;  // glossiness
    PPMTexture *metallic;   // reflectivity
    PPMTexture *diffuse;
    Vec3d diffuseColor;
};

}  // namespace rt

#endif
