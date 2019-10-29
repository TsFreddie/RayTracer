/*
 * BlinnPhong.cpp
 *
 *
 */

#include "BlinnPhong.h"



namespace rt{

BlinnPhong::BlinnPhong() {
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

Vec3f BlinnPhong::Shade(Scene *scene, Hit hit) {
    return this->diffuse;
}

} //namespace rt
