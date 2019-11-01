/*
 * Scene.h
 *
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "rapidjson/document.h"

#include "core/LightSource.h"
#include "core/Shape.h"
#include "math/geometry.h"


using namespace rapidjson;

namespace rt{

class Scene {
public:

	Scene(){};

	void createScene(Value& scenespecs);
	std::vector<Shape*>::iterator itShapeBegin() { return shapes.begin(); }
	std::vector<Shape*>::iterator itShapeEnd() { return shapes.end(); }

	std::vector<LightSource*>::iterator itLightBegin() { return lightSources.begin(); }
	std::vector<LightSource*>::iterator itLightEnd() { return lightSources.end(); }

	Vec3d getBackgroundColor() { return backgroundColor; }

	~Scene();

private:

	std::vector<LightSource*> lightSources;
	std::vector<Shape*> shapes;
	Vec3d backgroundColor;

};

} //namespace rt



#endif /* SCENE_H_ */
