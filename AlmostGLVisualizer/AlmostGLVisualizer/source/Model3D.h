#ifndef Model3D_h__
#define Model3D_h__

#include "Triangle3D.h"
#include "Material.h"
#include <vector>
#include <string>
#include <fstream>
#include <limits>

/*
    3D model wrapper

    Class holds information about 3D models defined in a text file in the format
    defined in UFRGS INF01009 Computer Graphics Course.

    Hold triangles, materials and bounding box related to the model.
*/

class Model3D
{
public:
    Model3D() {};
    Model3D(const std::string& filename);
    ~Model3D() {};
    bool loadFromFile(const std::string& filename);
    void updateBoundingBox(Vector3f v);

    Triangle3D triangle(int index) { return triangles[index]; }
    Material getMaterial(int index) { return materials[index]; }
public:
    std::vector<Triangle3D> triangles;
    std::vector<Material> materials;
    Material material;
    Vector3f min, max;
};

#endif // Model3D_h__