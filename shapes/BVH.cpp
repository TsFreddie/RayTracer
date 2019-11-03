/*
 * BVH.cpp
 *
 *
 */
#include <algorithm>
#include <random>

#include "BVH.h"

using namespace std;

namespace rt {
// comparators
bool compareBoundX(Shape *a, Shape *b) {
    return a->getBound().min.x < b->getBound().min.x;
}

bool compareBoundY(Shape *a, Shape *b) {
    return a->getBound().min.y < b->getBound().min.y;
}

bool compareBoundZ(Shape *a, Shape *b) {
    return a->getBound().min.z < b->getBound().min.z;
}

BVH::BVH(std::vector<Shape *> shapes, int begin, int end) {
    left = NULL;
    right = NULL;

    int nShape = end - begin;
    if (nShape == 1) {
        left = shapes[begin];
        mergeBound(left->getBound());
    } else if (nShape == 2) {
        left = shapes[begin];
        right = shapes[begin+1];
        mergeBound(left->getBound());
        mergeBound(right->getBound());
    } else {
        // Random x / y axis
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> rand(1, 2);
        int axis = rand(rng);

        switch (axis) {
            case 1:
                std::sort(shapes.begin() + begin, shapes.begin() + end,
                          compareBoundX);
                break;
            case 2:
                std::sort(shapes.begin() + begin, shapes.begin() + end,
                          compareBoundY);
                break;
            case 3:
                std::sort(shapes.begin() + begin, shapes.begin() + end,
                          compareBoundZ);
                break;
            default:
                throw("Unexpected random behaviour.");
        }

        int mid = begin + nShape / 2;
        left = new BVH(shapes, begin, mid);
        right = new BVH(shapes, mid, end);
        mergeBound(left->getBound());
        mergeBound(right->getBound());
    }
}

double BVH::intersect(Ray ray, Hit *hit) {
    // TODO: redo
    double t1, t2, t3, t4, t5, t6;
    t1 = (bound.min.x - ray.origin.x) * ray.invDir.x;
    t2 = (bound.max.x - ray.origin.x) * ray.invDir.x;
    t3 = (bound.min.y - ray.origin.y) * ray.invDir.y;
    t4 = (bound.max.y - ray.origin.y) * ray.invDir.y;
    t5 = (bound.min.z - ray.origin.z) * ray.invDir.z;
    t6 = (bound.max.z - ray.origin.z) * ray.invDir.z;

    double tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    double tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    if (tmax < 0) {
        return -1;
    }

    if (tmin > tmax) {
        return -1;
    }

    // If hit this BVH
    Hit *leftHit = NULL;
    Hit *rightHit = NULL;
    if (hit != NULL) {
        leftHit = new Hit();
        rightHit = new Hit();
    }

    double isLeftHit = (left != NULL) ? left->intersect(ray, leftHit) : -1;
    double isRightHit = (right != NULL) ? right->intersect(ray, rightHit) : -1;

    double nearDist = -1;
    Hit *nearHit = NULL;
    if (isLeftHit >= 0 && isRightHit >= 0) {
        if (isLeftHit < isRightHit) {
            nearDist = isLeftHit;
            nearHit = leftHit;
        } else {
            nearDist = isRightHit;
            nearHit = rightHit;
        }
    } else if (isLeftHit >= 0) {
        nearDist = isLeftHit;
        nearHit = leftHit;
    } else if (isRightHit >= 0) {
        nearDist = isRightHit;
        nearHit = rightHit;
    }

    if (hit != NULL) {
        if (nearHit != NULL) {
            hit->distance = nearHit->distance;
            hit->normal = nearHit->normal;
            hit->point = nearHit->point;
            hit->shape = nearHit->shape;
            hit->uv = nearHit->uv;
        }
        delete leftHit;
        delete rightHit;
        nearHit = NULL;
        leftHit = NULL;
        rightHit = NULL;
    }

    return nearDist;
}

BVH::~BVH() {
    if (left != NULL && typeid(left) == typeid(BVH())) delete left;
    if (right != NULL && typeid(right) == typeid(BVH())) delete right;
    left = NULL;
    right = NULL;
}

}  // namespace rt