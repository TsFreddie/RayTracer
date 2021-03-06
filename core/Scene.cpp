/*
 * Scene.cpp
 *
 */
#include "Scene.h"
#include "samplers/Jittered.h"
#include "samplers/Random.h"

namespace rt {

Scene::Scene() {
    bvh = NULL;
    backgroundColor = Vec3d(0);
    ambientIntensity = Vec3d(0);
    sampler = NULL;
}

/**
 * Parses json scene object to generate scene to render
 *
 * @param scenespecs the json scene specificatioon
 */
void Scene::createScene(Value& sceneSpecs) {
    //----------parse json object to populate scene-----------
    bool useBVH = true;
    if (sceneSpecs.HasMember("bvh") && sceneSpecs["bvh"].IsBool()) {
        useBVH = sceneSpecs["bvh"].GetBool();
    }

    // Populate shapes
    if (sceneSpecs.HasMember("shapes")) {
        Value& shapeSpecs = sceneSpecs["shapes"];
        SizeType nshapes = shapeSpecs.Size();
        for (SizeType i = 0; i < nshapes; i++) {
            Value& curShapeSpec = shapeSpecs[i];
            Shape* newShape = Shape::createShape(curShapeSpec, useBVH);
            if (newShape != NULL) shapes.push_back(newShape);
        }
    }

    // Populate lightsources
    if (sceneSpecs.HasMember("lightsources")) {
        Value& lightSpecs = sceneSpecs["lightsources"];
        SizeType nlights = lightSpecs.Size();
        for (SizeType i = 0; i < nlights; i++) {
            Value& curLightSpec = lightSpecs[i];
            LightSource* newLight = LightSource::createLight(curLightSpec);
            if (newLight != NULL) lightSources.push_back(newLight);
        }
    }

    // Set background color
    if (sceneSpecs.HasMember("backgroundcolor")) {
        Value& bcSpecs = sceneSpecs["backgroundcolor"];
        backgroundColor = Vec3d(bcSpecs[0].GetDouble(), bcSpecs[1].GetDouble(),
                                bcSpecs[2].GetDouble());
    }

    if (sceneSpecs.HasMember("ambientintensity")) {
        Value& acSpecs = sceneSpecs["ambientintensity"];
        ambientIntensity = Vec3d(acSpecs[0].GetDouble(), acSpecs[1].GetDouble(),
                                 acSpecs[2].GetDouble());
    }

    if (useBVH) {
        bvh = new BVH(shapes, 0, (int)shapes.size());
    }

    int nsamples = 1;
    bool isRandom = false;
    if (sceneSpecs.HasMember("nsamples") && sceneSpecs["nsamples"].IsInt()) {
        nsamples = sceneSpecs["nsamples"].GetInt();
    }

    if (sceneSpecs.HasMember("sampler") && sceneSpecs["sampler"].IsString()) {
        std::string sampler = sceneSpecs["sampler"].GetString();
        if (sampler.compare("random") == 0) {
            isRandom = true;
        }
    }

    if (isRandom) {
        sampler = new Random(nsamples, 1);
    } else {
        sampler = new Jittered(nsamples, 1);
    }
}

Sampler* Scene::getSampler() {
    if (sampler == NULL) {
        sampler = new Jittered(4, 1);
    }

    return sampler;
}

Scene::~Scene() {
    if (bvh != NULL) delete bvh;
    bvh = NULL;

    if (sampler != NULL) delete sampler;
    sampler = NULL;

    for (auto it = shapes.begin(); it != shapes.end(); ++it) {
        delete *it;
    }

    for (auto it = lightSources.begin(); it != lightSources.end(); ++it) {
        delete *it;
    }
}

}  // namespace rt
