/*
 * LightSource.cpp
 *
 */
#include "LightSource.h"
#include "lights/PointLight.h"

namespace rt {

LightSource::LightSource(Vec3d position, Vec3d intensity) {
    this->position = position;
    this->intensity = intensity;
}

LightSource* LightSource::createLight(Value& lightSpec) {
    std::string lightType = lightSpec["type"].GetString();

    LightSource* newLight = NULL;
    if (lightType.compare("pointlight") == 0) {
        if (!lightSpec.HasMember("position") ||
            !lightSpec["position"].IsArray() ||
            lightSpec["position"].Size() < 3 ||
            !lightSpec.HasMember("intensity") ||
            !lightSpec["intensity"].IsArray() ||
            lightSpec["intensity"].Size() < 3) {
            return NULL;
        }

        auto position = lightSpec["position"].GetArray();
        auto intensity = lightSpec["intensity"].GetArray();
        newLight = new PointLight(
            Vec3d(position[0].GetDouble(), position[1].GetDouble(),
                  position[2].GetDouble()),
            Vec3d(intensity[0].GetDouble(), intensity[1].GetDouble(),
                  intensity[2].GetDouble()));
    }

    return newLight;
}

}  
