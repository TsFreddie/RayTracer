/*
 * BlinnPhong.cpp
 *
 *
 */

#include "BlinnPhong.h"



namespace rt{

BlinnPhong::BlinnPhong() {
    this->ks = 0;
    this->kd = 0;
    this->specular = 0;
    this->diffuse = Vec3f(1,1,1);
}

BlinnPhong::BlinnPhong(float ks, float kd, float specular):BlinnPhong() {
    this->ks = ks;
    this->kd = kd;
    this->specular = specular;
}

void BlinnPhong::setDiffuse(Vec3f color) {
    this->diffuse = color;
}

} //namespace rt
