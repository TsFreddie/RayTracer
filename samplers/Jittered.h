/*
 * Jittered.h
 *
 */

#include <random>
#include "core/Sampler.h"

#ifndef JITTERED_H_
#define JITTERED_H_

namespace rt {

class Jittered : public Sampler {
   public:
    Jittered(int nsamples, double size);
    void reset(Vec3d normal);
    int count();
    Vec3d next();

   private:
    Vec3d normal;
    Vec3d u;
    Vec3d v;
    int nsamples;
    int dim;
    double step;
    double halfSize;
    double size;
    int ci;
    int cj;
    std::random_device dev;
    std::mt19937 rng;
};

}  // namespace rt

#endif