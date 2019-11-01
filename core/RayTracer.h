/*
 * RayTracer.h
 *
 */
#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "core/Camera.h"
#include "core/RayHitStructs.h"
#include "core/Scene.h"
#include "math/geometry.h"

namespace rt {

/*
 * Raytracer class declaration
 */
class RayTracer {
   public:
    RayTracer();

    static Ray createPrimRay(Camera* camera, int x, int y);
    static Ray createShadowRay(Hit hit, Vec3d lightPos);
    static Ray createSecondaryRay(Ray inRay, Hit hit, float randomness);

    static Vec3d traceRay(Scene* scene, Ray ray, int nbounce);

    static Vec3d* render(Camera* camera, Scene* scene, int nbounces);
    static Vec3d* tonemap(Vec3d* pixelbuffer, int width, int height);

   private:
};

}  

#endif
