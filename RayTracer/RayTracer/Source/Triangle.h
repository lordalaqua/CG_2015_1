#pragma once

#include "Vector.h"
#include "Object.h"
/*
    3D triangle (semi-plane) wrapper
    Stores information about a triangle in 3D space for rendering,
    such as vertices position, normal vector and an index
    for the vertex material.
*/
namespace YAML { class Node; }
class Triangle : public Object
{
public:
    Triangle() {};
    Triangle(const Triangle& other);
    Triangle(const YAML::Node& node);
    Hit intersect(const Ray &ray) const;
    static Hit intersect(const Object* o, const Ray& ray, const Vector3f& v0,
        const Vector3f& v1, const Vector3f& v2);
public:
    Vector3f vertex[3];
    Vector3f normal[3];
    Vector2f texture[3];
    int material_index[3];
    Vector3f face_normal;
};