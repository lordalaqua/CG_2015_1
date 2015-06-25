#pragma once

#include "Vector3f.h"

class Ray {
public:
    /* Dir is assumed to be normalized */
    Ray(const Vector3f& origin, const Vector3f& dir)
        : origin(origin), direction(dir) {
    }

    Vector3f at(double t) const { return origin + t * direction; }

    Vector3f& getOrigin() { return origin; }
    Vector3f& getDirection() { return direction; }

private:
    Vector3f origin;
    Vector3f direction;
};