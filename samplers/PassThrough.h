/*
 * PassThrough.h
 *
 */

#include "core/Sampler.h"

#ifndef PASSTHROUGH_H_
#define PASSTHROUGH_H_

namespace rt {

class PassThrough : public Sampler {
   public:
    void reset(Vec3d normal) {}
    int count() { return 1; }
    Vec3d next() { return Vec3d(0,0,0); }
};

}  // namespace rt

#endif