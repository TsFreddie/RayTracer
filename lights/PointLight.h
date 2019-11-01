/*
 * PointLight.h
 *
 *
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "core/LightSource.h"
#include "math/geometry.h"

namespace rt {

class PointLight : public LightSource {
   public:
    PointLight(Vec3d position, Vec3d intensity);

   private:
};

}  

#endif
