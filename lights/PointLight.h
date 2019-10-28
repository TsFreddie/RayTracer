/*
 * PointLight.h
 *
 *
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "math/geometry.h"
#include "core/LightSource.h"


namespace rt{

class PointLight: public LightSource{

public:
    PointLight(Vec3f position, Vec3f intensity);
private:

};



} //namespace rt




#endif /* POINTLIGHT_H_ */
