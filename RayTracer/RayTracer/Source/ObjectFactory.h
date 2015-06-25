#pragma once

#include "Object.h"
#include <vector>

#include "Model.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Tetrahedron.h"
#include "Torus.h"

namespace YAML { class Node; }

class ObjectFactory
{
public:
    static std::vector<Object*> createObject(const YAML::Node& node)
    {
        std::vector<Object*> objects;
        std::string objectType;
        node["type"] >> objectType;

        if (objectType == "sphere") 
        {
            objects.push_back(new Sphere(node));
        }
        else if (objectType == "cylinder")
        {
            objects.push_back(new Cylinder(node));
        }
        else if (objectType == "tetrahedron")
        {
            objects.push_back(new Tetrahedron(node));
        }
        else if (objectType == "torus")
        {
            objects.push_back(new Torus(node));
        }
        else if (objectType == "model") {
            Model* m = new Model(node);
            for (unsigned i = 0; i < m->triangles.size(); ++i)
            {
                objects.push_back(new Triangle(m->triangles[i]));
            }                
        }

        for (auto& obj : objects)
            obj->readMaterial(node["material"]);

        return objects;
    }
private:
    ObjectFactory();
};