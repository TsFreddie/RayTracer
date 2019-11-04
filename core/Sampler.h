/*
 * Sampler.h
 *
 */

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include "math/geometry.h"

namespace rt {

class Sampler {
   public:
    virtual void reset(Vec3d normal) = 0;
    
    virtual int count() = 0;
    virtual Vec3d next() = 0;
};

}  // namespace rt

#endif
