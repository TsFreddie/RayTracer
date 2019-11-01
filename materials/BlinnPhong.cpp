/*
 * BlinnPhong.cpp
 *
 *
 */

#include "BlinnPhong.h"
#include "core/LightSource.h"
#include <windows.h>



namespace rt{

BlinnPhong::BlinnPhong(): Material() {
    this->ka = 0;
    this->ks = 0;
    this->kd = 0;
    this->specular = 0;
    this->diffuse = Vec3f(1,1,1);
}

BlinnPhong::BlinnPhong(float ka, float ks, float kd, float specular): BlinnPhong() {
    this->ka = ka;
    this->ks = ks;
    this->kd = kd;
    this->specular = specular;
}

void BlinnPhong::setDiffuse(Vec3f color) {
    this->diffuse = color;
}

Vec3f BlinnPhong::Shade(LightSource *light, Hit hit) {
    // TODO: camera transformation
    Vec3f view = hit.view;
    Vec3f normal = hit.normal;
    Vec3f L = (light->getPosition() - hit.point).normalize();
    
    float NL = normal.dotProduct(L);
    float intensity = (NL > 0.0f) ? NL : 0.0f;

    Vec3f color = diffuse * light->getIntensity() * intensity;

    Vec3f H = (L + view).normalize();

    float NH = normal.dotProduct(H);

    float spec = powf((NH > 0.0f) ? NH : 0.0f, specular);

    Vec3f specColor = light->getIntensity() * spec;

    return color * kd + specColor * ks; 
}

} //namespace rt
