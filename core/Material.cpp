/*
 * Material.cpp
 *
 */
#include "Material.h"
#include "materials/BlinnPhong.h"
#include "parsers/PPMTexture.h"

namespace rt {

Material::Material() {
    transmit = new PPMTexture();
    metallic = new PPMTexture();
    roughness = new PPMTexture();
    diffuse = new PPMTexture();
}

double Material::getTransmit(double u, double v) { return transmit->getValue(u, v); }
double Material::getRoughness(double u, double v) { return roughness->getValue(u, v); }
double Material::getMetallic(double u, double v) { return metallic->getValue(u, v); }
Vec3d Material::getDiffuse(double u, double v) {
    return diffuseColor * diffuse->getChannels(u, v);
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

    if (matSpec.HasMember("diffusecolor") &&
        matSpec["diffusecolor"].IsArray() &&
        matSpec["diffusecolor"].Size() >= 3) {
        auto diffuseArray = matSpec["diffusecolor"].GetArray();
        newMat->diffuseColor =
            Vec3d(diffuseArray[0].GetDouble(), diffuseArray[1].GetDouble(),
                  diffuseArray[2].GetDouble());
    } else {
        newMat->diffuseColor = Vec3d(1, 1, 1);
    }

    if (matSpec.HasMember("diffuse") && matSpec["diffuse"].IsString()) {
        const char* str = matSpec["diffuse"].GetString();
        if (strcmp(str, "checkerboard") == 0) {
            newMat->diffuse->checkerboard();
        } else {
            newMat->diffuse->file(str);
        }
    }

    // TODO: property texture
    if (matSpec.HasMember("metallic") && matSpec["metallic"].IsNumber()) {
        newMat->metallic->solid(matSpec["metallic"].GetDouble());
    } else {
        newMat->metallic->solid(0.5);
    }

    if (matSpec.HasMember("roughness") && matSpec["roughness"].IsNumber()) {
        newMat->roughness->solid(matSpec["roughness"].GetDouble());
    } else {
        newMat->roughness->solid(0);
    }

    if (matSpec.HasMember("transmit") && matSpec["transmit"].IsNumber()) {
        newMat->transmit->solid(matSpec["transmit"].GetDouble());
    } else {
        newMat->transmit->solid(0);
    }

    return newMat;
}

Material::~Material() {
    if (diffuse) delete diffuse;
    if (metallic) delete metallic;
    if (roughness) delete roughness;
    if (transmit) delete transmit;
}

}  // namespace rt
