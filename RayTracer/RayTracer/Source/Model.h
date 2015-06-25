#pragma once

#include "Triangle.h"
#include "Material.h"
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include "Matrix.h"
#include "Vector.h"

namespace YAML { class Node; }

class Model
{
public:
    Model() {};
    Model(const std::string& filename);
    Model(const YAML::Node& node);
    ~Model() {};
    bool loadFromFile(const std::string& filename);
    void updateBoundingBox(Vector3f v);

    Triangle triangle(int index) { return triangles[index]; }
    Material getMaterial(int index) { return materials[index]; }
public:
    Vector3f position;
    float scale;
    bool smooth;
    std::vector<Triangle> triangles;
    std::vector<Material> materials;
    Material material;
    Vector3f min, max;
};