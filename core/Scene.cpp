/*
 * Scene.cpp
 *
 */
#include "Scene.h"




namespace rt{

/**
 * Parses json scene object to generate scene to render
 *
 * @param scenespecs the json scene specificatioon
 */
void Scene::createScene(Value& sceneSpecs){

	//----------parse json object to populate scene-----------

	// Populate shapes
	if (sceneSpecs.HasMember("shapes") && sceneSpecs["shapes"].IsArray()) {
		Value& shapeSpecs = sceneSpecs["shapes"];
		SizeType nshapes = shapeSpecs.Size();
		for (SizeType i = 0; i < nshapes; i++) {
			Value& curShapeSpec = shapeSpecs[i];
			Shape* newShape = Shape::createShape(curShapeSpec);
			if (newShape != NULL) shapes.push_back(newShape);
		}
	}

	// Populate lightsources

	// Set background color
	if (sceneSpecs.HasMember("backgroundcolor") &&
		sceneSpecs["backgroundcolor"].IsArray() &&
		sceneSpecs["backgroundcolor"].Size() >= 3) {
		
		Value& bcSpecs = sceneSpecs["backgroundcolor"];
		backgroundColor = Vec3f(bcSpecs[0].GetFloat(), 
								bcSpecs[1].GetFloat(),
								bcSpecs[2].GetFloat());
	} else {
		backgroundColor = Vec3f(0,0,0);
	}
}

std::vector<Shape*>::iterator Scene::itShapeBegin() {
	return shapes.begin();
}

std::vector<Shape*>::iterator Scene::itShapeEnd() {
	return shapes.end();
}

Scene::~Scene() {
	for (auto it = shapes.begin(); it != shapes.end(); ++it) {
		delete *it;
	}

	for (auto it = lightSources.begin(); it != lightSources.end(); ++it) {
		delete *it;
	}
}

} //namespace rt
