/*
 * BlinnPhong.cpp
 *
 *
 */

#include "BlinnPhong.h"
#include "core/LightSource.h"

namespace rt{

BlinnPhong::BlinnPhong(): Material() {
    this->ka = 0;
    this->ks = 0;
    this->kd = 0;
    this->specular = 0;
    this->diffuse = Vec3d(1,1,1);
}

BlinnPhong::BlinnPhong(double ka, double ks, double kd, double specular): BlinnPhong() {
    this->ka = ka;
    this->ks = ks;
    this->kd = kd;
    this->specular = specular;
}

void BlinnPhong::setDiffuse(Vec3d color) {
    this->diffuse = color;
}

Vec3d BlinnPhong::Shade(LightSource *light, Hit hit) {
    // TODO: camera transformation
    Vec3d view = hit.view;
    Vec3d normal = hit.normal;
    Vec3d L = (light->getPosition() - hit.point).normalize();
    
    double NL = normal.dotProduct(L);
    double intensity = (NL > 0.0) ? NL : 0.0;

    Vec3d color = diffuse * light->getIntensity() * intensity;

    Vec3d H = (L + view).normalize();

    double NH = normal.dotProduct(H);

    double spec = pow((NH > 0.0) ? NH : 0.0, specular);

    Vec3d specColor = light->getIntensity() * spec;

    return color * kd + specColor * ks; 
}

} //namespace rt
