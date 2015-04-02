#ifndef Model3D_h__
#define Model3D_h__

#include "Triangle3D.h"
#include "Material.h"
#include <vector>
#include <string>
#include <fstream>
#include <limits>

class Model3D
{
public:
    Model3D() {};
    Model3D(const std::string& filename);
    ~Model3D() {};
    bool loadFromFile(const std::string& filename);
    void updateBoundingBox(Vector3f v);

    Triangle3D triangle(int index) { return triangles[index]; }
    Material material(int index) { return materials[index]; }
public:
    std::vector<Triangle3D> triangles;
    std::vector<Material> materials;

    Vector3f min, max;
};

#endif // Model3D_h__