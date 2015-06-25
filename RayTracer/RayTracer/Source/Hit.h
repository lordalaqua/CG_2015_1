#pragma once
#include "Vector.h"
#include <limits>
class Object;
const double INF = std::numeric_limits<double>::max();
class Hit {
public:
    /* N is assumed to be normalized here */
    Hit(const Object* object = nullptr, double distance = INF, 
        Vector3f normal = { 0, 0, 0 })
        : object(object), distance(distance), normal(normal) {
        // empty
    }

    static Hit None() { return Hit(nullptr); }

    bool isHit() const { return object != nullptr; }

    bool isNoHit() const { return object == nullptr; }

    bool operator<(const Hit& h) const { return distance < h.distance; }

    double getDistance() const { return distance; }

    const Vector3f& getNormal() const { return normal; }

    void setNormal(const Vector3f& v) { normal = v; }

    const Object* getObject() const { return object; }

    void setObject(const Object* object) { object = object; }

    operator bool() const { return object != nullptr; }

private:
    const Object* object;
    double distance;
    Vector3f normal;
};