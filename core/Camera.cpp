/*
 * Camera.cpp
 *
 */
#include <iostream>

#include "Camera.h"
#include "samplers/Jittered.h"
#include "samplers/PassThrough.h"
#include "samplers/Random.h"

namespace rt {

Camera::Camera(int height, int width, double fov)
    : height(height), width(width) {
    this->setFov(fov);
    sampler = NULL;
    focusdistance = 1.0;
}
Camera::~Camera() {
    if (sampler != NULL) delete sampler;
    sampler = NULL;
};

/**
 * Factory function that returns camera subclass based on camera specifications
 *
 * @param cameraSpecs camera specifications json object
 *
 * @return camera subclass instance
 *
 */
Camera* Camera::createCamera(Value& cameraSpecs) {
    // check if cameratype is defined

    if (!cameraSpecs.HasMember("type")) {
        std::cerr << "Camera type not specified" << std::endl;
        exit(-1);
    }

    std::string cameraType = cameraSpecs["type"].GetString();

    Camera* newCamera =
        new Camera(cameraSpecs["width"].GetInt(), cameraSpecs["height"].GetInt(),
               cameraSpecs["fov"].GetInt());

    // return camera object based on camera specs
    if (cameraType.compare("pinhole") == 0) {
        newCamera->sampler = new PassThrough();
    } else if (cameraType.compare("thinlen") == 0) {
        int nsamples = 32;
        double size = 0.1;
        bool isRandom = false;

        if (cameraSpecs.HasMember("sampler") && cameraSpecs["sampler"].IsString()) {
            std::string sampler = cameraSpecs["sampler"].GetString();
            if (sampler.compare("random") == 0) {
                isRandom = true;
            }
        }

        if (cameraSpecs.HasMember("nsamples") && cameraSpecs["nsamples"].IsInt()) {
            nsamples = cameraSpecs["nsamples"].GetInt();
        }
        if (cameraSpecs.HasMember("size") && cameraSpecs["size"].IsNumber()) {
            size = cameraSpecs["size"].GetDouble();
        }

        if (isRandom) {
            newCamera->sampler = new Random(nsamples, size);
        } else {
            newCamera->sampler = new Jittered(nsamples, size);
        }
    } else {
        delete newCamera;
        return NULL;
    }

    return newCamera;
}

Sampler* Camera::getSampler() {
    if (sampler == NULL) {
        sampler = new PassThrough();
    }

    return sampler;
}
}  // namespace rt
