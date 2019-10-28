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

    LightSource(Vec3f position, Vec3f intensity);

    virtual ~LightSource() {};

    static LightSource* createLight(Value& lightSpec);

    Vec3f getPosition() { return position; };
    Vec3f getIntensity() { return intensity; }

private:

    Vec3f position;
    Vec3f intensity;

};

} //namespace rt



#endif /* LIGHTSOURCE_H_ */
