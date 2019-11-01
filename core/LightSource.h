/*
 * LightSource.h
 *
 */

#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "math/geometry.h"
#include "rapidjson/document.h"
using namespace rapidjson;

namespace rt{

class LightSource{
public:

    LightSource(Vec3d position, Vec3d intensity);

    virtual ~LightSource() {};

    static LightSource* createLight(Value& lightSpec);

    Vec3d getPosition() { return position; };
    Vec3d getIntensity() { return intensity; }

private:

    Vec3d position;
    Vec3d intensity;

};

} //namespace rt



#endif /* LIGHTSOURCE_H_ */
