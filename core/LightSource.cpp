/*
 * LightSource.cpp
 *
 */
#include "LightSource.h"
#include "samplers/Jittered.h"
#include "samplers/PassThrough.h"
#include "samplers/Random.h"

namespace rt {

LightSource::LightSource(Vec3d position, Vec3d intensity) {
    this->position = position;
    this->intensity = intensity;
    sampler = NULL;
}

LightSource* LightSource::createLight(Value& lightSpec) {
    std::string lightType = lightSpec["type"].GetString();

    LightSource* newLight = NULL;

    auto position = lightSpec["position"].GetArray();
    auto intensity = lightSpec["intensity"].GetArray();
    newLight = new LightSource(
        Vec3d(position[0].GetDouble(), position[1].GetDouble(),
              position[2].GetDouble()),
        Vec3d(intensity[0].GetDouble(), intensity[1].GetDouble(),
              intensity[2].GetDouble()));

    if (lightType.compare("pointlight") == 0) {
        newLight->sampler = new PassThrough();
    } else if (lightType.compare("arealight") == 0) {
        int nsamples = 32;
        double size = 0.1;
        bool isRandom = false;
        if (lightSpec.HasMember("nsamples") && lightSpec["nsamples"].IsInt()) {
            nsamples = lightSpec["nsamples"].GetInt();
        }
        if (lightSpec.HasMember("size") && lightSpec["size"].IsNumber()) {
            size = lightSpec["size"].GetDouble();
        }

        if (lightSpec.HasMember("sampler") && lightSpec["sampler"].IsString()) {
            std::string sampler = lightSpec["sampler"].GetString();
            if (sampler.compare("random") == 0) {
                isRandom = true;
            }
        }

        if (isRandom) {
            newLight->sampler = new Random(nsamples, size);
        } else {
            newLight->sampler = new Jittered(nsamples, size);
        }
    } else {
        delete newLight;
        return NULL;
    }

    return newLight;
}  // namespace rt

Sampler* LightSource::getSampler() {
    if (sampler == NULL) {
        sampler = new PassThrough();
    }

    return sampler;
}

LightSource::~LightSource() {
    if (sampler != NULL) delete sampler;
    sampler = NULL;
}
}  // namespace rt
