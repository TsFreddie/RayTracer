/*
 * BlinnPhong.cpp
 *
 *
 */

#include "BlinnPhong.h"
#include "core/LightSource.h"

namespace rt {

BlinnPhong::BlinnPhong() : Material() {
    this->ka = 1;
    this->ks = 0;
    this->kd = 0;
    this->specular = 0;
}

BlinnPhong::BlinnPhong(double ka, double ks, double kd, double specular)
    : BlinnPhong() {
    this->ka = ka;
    this->ks = ks;
    this->kd = kd;
    this->specular = specular;
}

double BlinnPhong::getAmbient() {
    return ka;
}

Vec3d BlinnPhong::Shade(LightSource *light, Hit hit, Vec3d view) {
    Vec3d normal = hit.normal;
    Vec3d L = (light->getPosition() - hit.point).normalize();

    double NL = normal.dotProduct(L);
    double intensity = (NL > 0.0) ? NL : 0.0;

    Vec3d color = getDiffuse(hit.uv) * light->getIntensity() * intensity;

    Vec3d H = (L + view).normalize();

    double HN = H.dotProduct(normal);

    double spec = pow((HN > 0.0) ? HN : 0.0, specular);

    Vec3d specColor = light->getIntensity() * spec;

    return color * kd + specColor * ks;
}

}  
