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
	std::vector<Shape*>::iterator itShapeBegin();
	std::vector<Shape*>::iterator itShapeEnd();

	~Scene();

private:

	std::vector<LightSource*> lightSources;
	std::vector<Shape*> shapes;
	Vec3f backgroundColor;

};

} //namespace rt



#endif /* SCENE_H_ */
