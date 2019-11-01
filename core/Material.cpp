/*
 * Material.cpp
 *
 */
#include "Material.h"
#include "materials/BlinnPhong.h"


namespace rt{

    Material::Material() {
        transmit = 0.0f;
        metallic = 0.5f;
        roughness = 0.0f;
    }

    Material* Material::createMaterial(Value& matSpec) {

        float ka = 0;
        float ks = 0;
        float kd = 0;
        float specular = 0;

        if (matSpec.HasMember("ka") && matSpec["ka"].IsNumber()) {
            ka= matSpec["ka"].GetFloat();
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
        BlinnPhong* newMat = new BlinnPhong(ka, ks, kd, specular);

        if (matSpec["diffusecolor"].IsArray() && matSpec["diffusecolor"].Size() >= 3) {
            auto diffuse = matSpec["diffusecolor"].GetArray();
            newMat->setDiffuse(Vec3f(diffuse[0].GetFloat(), diffuse[1].GetFloat(), diffuse[2].GetFloat()));
        }

        if (matSpec.HasMember("metallic") && matSpec["metallic"].IsNumber()) {
            newMat->metallic = matSpec["metallic"].GetFloat();
        }

        if (matSpec.HasMember("roughness") && matSpec["roughness"].IsNumber()) {
            newMat->roughness = matSpec["roughness"].GetFloat();
        }

        if (matSpec.HasMember("transmit") && matSpec["transmit"].IsNumber()) {
            newMat->transmit = matSpec["transmit"].GetFloat();
        }

        return newMat; 
    }

} //namespace rt


