/*
 * Material.cpp
 *
 */
#include "Material.h"
#include "materials/BlinnPhong.h"


namespace rt{

Material* Material::createMaterial(Value& matSpec) {
    float ka = 0;
    float ks = 0;
    float kd = 0;
    float specular = 0;

    if (matSpec.HasMember("ka") && matSpec["ka"].IsNumber()) {
        ka = matSpec["ka"].GetFloat();
    }
    
    if (matSpec.HasMember("ks") && matSpec["ks"].IsNumber()) {
        ks = matSpec["ks"].GetFloat();
    }

    if (matSpec.HasMember("kd") && matSpec["kd"].IsNumber()) {
        kd = matSpec["kd"].GetFloat();
    }

    if (matSpec.HasMember("specularexponent") && matSpec["specularexponent"].IsNumber()) {
        specular = matSpec["specularexponent"].GetFloat();
    }

    // Currently only BlinnPhong
    // TODO: not a good factory, fix it.
    BlinnPhong* newMat = new BlinnPhong(ka, ks, kd, specular);

    if (matSpec["diffusecolor"].IsArray() && matSpec["diffusecolor"].Size() >= 3) {
        auto diffuse = matSpec["diffusecolor"].GetArray();
        newMat->setDiffuse(Vec3f(diffuse[0].GetFloat(), diffuse[1].GetFloat(), diffuse[2].GetFloat()));
    }

    return newMat; 
}

} //namespace rt


