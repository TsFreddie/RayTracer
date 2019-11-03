/*
 * Scene.cpp
 *
 */
#include "Scene.h"

namespace rt {

/**
 * Parses json scene object to generate scene to render
 *
 * @param scenespecs the json scene specificatioon
 */
void Scene::createScene(Value& sceneSpecs) {
    //----------parse json object to populate scene-----------

    // Populate shapes
    if (sceneSpecs.HasMember("shapes")) {
        Value& shapeSpecs = sceneSpecs["shapes"];
        SizeType nshapes = shapeSpecs.Size();
        for (SizeType i = 0; i < nshapes; i++) {
            Value& curShapeSpec = shapeSpecs[i];
            Shape* newShape = Shape::createShape(curShapeSpec);
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
    } else {
        backgroundColor = Vec3d(0, 0, 0);
    }

    bvhVector.push_back(new BVH(shapes, 0, (int)shapes.size()));
}

std::vector<Shape*>::iterator Scene::itShapeBegin() {
    if (bvhVector.size() > 0) {
        return bvhVector.begin();
    }
    return shapes.begin();
}
std::vector<Shape*>::iterator Scene::itShapeEnd() {
    if (bvhVector.size() > 0) {
        return bvhVector.end();
    }
    return shapes.end();
}

Scene::~Scene() {
    for (auto it = bvhVector.begin(); it != bvhVector.end(); ++it) {
        delete *it;
    }

    for (auto it = shapes.begin(); it != shapes.end(); ++it) {
        delete *it;
    }

    for (auto it = lightSources.begin(); it != lightSources.end(); ++it) {
        delete *it;
    }
}

}  // namespace rt
