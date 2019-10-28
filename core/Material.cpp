/*
 * Material.cpp
 *
 */
#include "Material.h"
#include "materials/BlinnPhong.h"


namespace rt{

Material* Material::createMaterial(Value& matSpec) {
    float ks = 0;
    float kd = 0;
    float specular = 0;
    
    if (matSpec["ks"].IsFloat()) {
        ks = matSpec["ks"].GetFloat();
    }

    if (matSpec["kd"].IsFloat()) {
        kd = matSpec["kd"].GetFloat();
    }

    if (matSpec["specularexponent"].IsFloat()) {
        specular = matSpec["specularexponent"].GetFloat();
    }

    // Currently only BlinnPhong
    // TODO: not a good factory, fix it.
    BlinnPhong* newMat = new BlinnPhong(ks, kd, specular);

    if (matSpec["diffusecolor"].IsArray() && matSpec["diffusecolor"].Size() >= 3) {
        auto diffuse = matSpec["diffusecolor"].GetArray();
        newMat->setDiffuse(Vec3f(diffuse[0].GetFloat(), diffuse[1].GetFloat(), diffuse[2].GetFloat()));
    }

    return newMat; 
}

} //namespace rt


