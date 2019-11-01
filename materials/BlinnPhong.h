/*
 * BlinnPhong.h
 *
 *
 */

#ifndef BLINNPHONG_H_
#define BLINNPHONG_H_

#include "core/Material.h"

#include "math/geometry.h"

namespace rt {

class BlinnPhong : public Material {
   public:
    BlinnPhong();
    BlinnPhong(double ka, double ks, double kd, double specular);
    void setDiffuse(Vec3d color);
    Vec3d Shade(LightSource *light, Hit hit, Vec3d view);

   private:
    double ka;
    double ks;
    double kd;
    double specular;
    Vec3d diffuse;
};

}  

#endif
