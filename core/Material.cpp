/*
 * Material.cpp
 *
 */
#include "Material.h"
#include "materials/BlinnPhong.h"

namespace rt {

Material::Material() {
    transmit = 0.0;
    metallic = 0.5;
    roughness = 0.0;
}

Material* Material::createMaterial(Value& matSpec) {
    double ka = 0;
    double ks = 0;
    double kd = 0;
    double specular = 0;

    if (matSpec.HasMember("ka") && matSpec["ka"].IsNumber()) {
        ka = matSpec["ka"].GetDouble();
    }

    if (matSpec.HasMember("ks") && matSpec["ks"].IsNumber()) {
        ks = matSpec["ks"].GetDouble();
    }

    if (matSpec.HasMember("kd") && matSpec["kd"].IsNumber()) {
        kd = matSpec["kd"].GetDouble();
    }

    if (matSpec.HasMember("specularexponent") &&
        matSpec["specularexponent"].IsNumber()) {
        specular = matSpec["specularexponent"].GetDouble();
    }

    // Currently only BlinnPhong
    BlinnPhong* newMat = new BlinnPhong(ka, ks, kd, specular);

    if (matSpec["diffusecolor"].IsArray() &&
        matSpec["diffusecolor"].Size() >= 3) {
        auto diffuseArray = matSpec["diffusecolor"].GetArray();
        newMat->diffuse = Vec3d(diffuseArray[0].GetDouble(), diffuseArray[1].GetDouble(),
                                 diffuseArray[2].GetDouble());
    }

    if (matSpec.HasMember("metallic") && matSpec["metallic"].IsNumber()) {
        newMat->metallic = matSpec["metallic"].GetDouble();
    }

    if (matSpec.HasMember("roughness") && matSpec["roughness"].IsNumber()) {
        newMat->roughness = matSpec["roughness"].GetDouble();
    }

    if (matSpec.HasMember("transmit") && matSpec["transmit"].IsNumber()) {
        newMat->transmit = matSpec["transmit"].GetDouble();
    }

    return newMat;
}

}  
