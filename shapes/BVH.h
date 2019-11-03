/*
 * BVH.h
 *
 *
 */

#ifndef BVH_H_
#define BVH_H_

#include <vector>
#include "core/Shape.h"

namespace rt {

class BVH : public Shape {
   public:
    BVH(std::vector<Shape *> shapes, int start, int end);

    double intersect(Ray ray, Hit *hit);

    ~BVH();

   private:
    Shape *left, *right;  // BVN is a shape too.
};

}  // namespace rt

#endif
