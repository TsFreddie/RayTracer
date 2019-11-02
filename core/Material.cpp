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
    transmit->solid(0);
    metallic = new PPMTexture();
    metallic->solid(0.5);
    roughness = new PPMTexture();
    roughness->solid(0);
    diffuse = new PPMTexture();
}

double Material::getTransmit(Vec2d uv) { return transmit->getValue(uv); }
double Material::getRoughness(Vec2d uv) { return roughness->getValue(uv); }
double Material::getMetallic(Vec2d uv) { return metallic->getValue(uv); }
Vec3d Material::getDiffuse(Vec2d uv) {
    return diffuseColor * diffuse->getChannels(uv);
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
        newMat->diffuse->file(matSpec["diffuse"].GetString());
    }

    // TODO: property texture
    if (matSpec.HasMember("metallic")) {
        if (matSpec["metallic"].IsNumber()) {
            newMat->metallic->solid(matSpec["metallic"].GetDouble());
        } else if (matSpec["metallic"].IsString()) {
            newMat->metallic->file(matSpec["metallic"].GetString());
        }
    }

    if (matSpec.HasMember("roughness")) {
        if (matSpec["roughness"].IsNumber()) {
            newMat->roughness->solid(matSpec["roughness"].GetDouble());
        } else if (matSpec["roughness"].IsString()) {
            newMat->roughness->file(matSpec["roughness"].GetString());
        }
    }

    if (matSpec.HasMember("transmit")) {
        if (matSpec["transmit"].IsNumber()) {
            newMat->transmit->solid(matSpec["transmit"].GetDouble());
        } else if (matSpec["transmit"].IsString()) {
            newMat->transmit->file(matSpec["transmit"].GetString());
        }
    }

    return newMat;
}

Material::~Material() {
    if (diffuse) delete diffuse;
    if (metallic) delete metallic;
    if (roughness) delete roughness;
    if (transmit) delete transmit;

    diffuse = NULL;
    metallic = NULL;
    roughness = NULL;
    transmit = NULL;
}

}  // namespace rt
