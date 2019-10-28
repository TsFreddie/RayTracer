/*
 * LightSource.cpp
 *
 */
#include "LightSource.h"
#include "lights/PointLight.h"

namespace rt{

    LightSource::LightSource(Vec3f position, Vec3f intensity) {
        this->position = position;
        this->intensity = intensity;
    }

    LightSource* LightSource::createLight(Value& lightSpec) {
        std::string lightType = lightSpec["type"].GetString();

        LightSource* newLight = NULL;
        if (lightType.compare("pointlight") == 0) {
            if (!lightSpec.HasMember("position") || !lightSpec["position"].IsArray() ||
                lightSpec["position"].Size() < 3 ||
                !lightSpec.HasMember("intensity") || !lightSpec["intensity"].IsArray() ||
                lightSpec["intensity"].Size() < 3) {

                return NULL;
            }

            auto position = lightSpec["position"].GetArray();
            auto intensity = lightSpec["intensity"].GetArray();
            newLight = new PointLight(
                Vec3f(
                    position[0].GetFloat(),
                    position[1].GetFloat(),
                    position[2].GetFloat()
                ),
                Vec3f(
                    intensity[0].GetFloat(),
                    intensity[1].GetFloat(),
                    intensity[2].GetFloat()
                )
            );
        }

        return newLight;
    }

} //namespace rt

