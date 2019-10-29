/*
 * BlinnPhong.h
 *
 *
 */

#ifndef BLINNPHONG_H_
#define BLINNPHONG_H_

#include "core/Material.h"

#include "math/geometry.h"


namespace rt{

class BlinnPhong: public Material{

public:
    BlinnPhong();
    BlinnPhong(float ka, float ks, float kd, float specular);
    void setDiffuse(Vec3f color);
    Vec3f Shade(LightSource *light, Hit hit);

private:
    float ka;
    float ks;
    float kd;
    float specular;
    Vec3f diffuse;
};



} //namespace rt



#endif /* BLINNPHONG_H_ */
