/*
 * Random.h
 *
 */

#include <random>
#include "core/Sampler.h"

#ifndef RANDOM_H_
#define RANDOM_H_

namespace rt {

class Random : public Sampler {
   public:
    Random(int nsamples, double size);
    void reset(Vec3d normal);
    int count();
    Vec3d next();

   private:
    Vec3d normal;
    Vec3d u;
    Vec3d v;
    int nsamples;
    double size;
    std::random_device dev;
    std::mt19937 rng;
};

}  // namespace rt

#endif