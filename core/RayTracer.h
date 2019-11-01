/*
 * RayTracer.h
 *
 */
#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "core/RayHitStructs.h"
#include "math/geometry.h"
#include "core/Camera.h"
#include "core/Scene.h"

namespace rt{



/*
 * Raytracer class declaration
 */
class RayTracer {
public:

	RayTracer();

	static Ray createPrimRay(Camera* camera, int x, int y);
	static Ray createShadowRay(Hit hit, Vec3f lightPos);
	
	static Vec3f traceRay(Scene *scene, Ray ray, int nbounce);

	static Vec3f* render(Camera* camera, Scene* scene, int nbounces);
	static Vec3f* tonemap(Vec3f* pixelbuffer, int width, int height);


private:


};




} //namespace rt



#endif /* RAYTRACER_H_ */

