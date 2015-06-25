#pragma once

#include "Object.h"

namespace YAML { class Node; }

class Sphere : public Object 
{
public:
    Sphere(const YAML::Node& node);

    Hit intersect(const Ray &ray) const;

    Vector2f getTextureCoord(Vector3f p) const;

private:
    /* Pre-computed variables for sphere rotation */
    Material m_Material;
    Vector3f position;
    double r;
};
