/*
 * Random.h
 *
 */

#include "samplers/Random.h"

typedef std::uniform_real_distribution<double> Rand;

namespace rt {

Random::Random(int nsamples, double size) {
    this->nsamples = nsamples;
    this->size = size;
    normal = Vec3d(0);
}

void Random::reset(Vec3d normal) {
    this->normal = normal;
    Vec3d any = Vec3d(normal.x + 1, normal.y, normal.z);
    u = normal.crossProduct(any).normalize();
    v = normal.crossProduct(u);
}

int Random::count() {
    return nsamples;
}

Vec3d Random::next() {
    Rand rand(-0.5, 0.5);
    double a = rand(rng);
    double b = rand(rng);
    return u * a * size + v * b * size;
}

}