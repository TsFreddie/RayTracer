/*
 * Jittered.h
 *
 */

#include "samplers/Jittered.h"

typedef std::uniform_real_distribution<double> Rand;

namespace rt {

Jittered::Jittered(int nsamples, double size) {
    dim = ((int)sqrt(nsamples));
    this->nsamples = dim * dim;
    if (nsamples != this->nsamples) {
        printf("NOTE: Using %d samples instead of %d.\n", this->nsamples, nsamples);
    }
    this->step = size / (double) dim;
    this->halfSize = size / 2.0;
    this->size = size;
    normal = Vec3d(0);
    ci = 0;
    cj = 0;
}

void Jittered::reset(Vec3d normal) {
    this->normal = normal;
    Vec3d any = Vec3d(normal.x + 1, normal.y, normal.z);
    u = normal.crossProduct(any).normalize();
    v = normal.crossProduct(u);
    ci = 0;
    cj = 0;
}

int Jittered::count() {
    return nsamples;
}

Vec3d Jittered::next() {
    Rand rand(0, 1);
    double a = -halfSize + ci * step + step * rand(rng);
    double b = -halfSize + cj * step + step * rand(rng);

    ++ci;
    if (ci >= dim) {
        ci = 0;
        ++cj;
    }

    return u * a + v * b;
}

}