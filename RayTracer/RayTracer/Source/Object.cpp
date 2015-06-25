#include "Object.h"
#include "yaml/yaml.h"
#include <iostream>

using namespace std;

Object::Object() 
{
}

Object::Object(const YAML::Node& node) 
{
    if (node.FindValue("material"))
        readMaterial(node["material"]);
}

Object::~Object() 
{
}

Vector3f Object::colorAt(const Vector3f& p) const {
    if (isTextured()) {
        Vector2f tc = getTextureCoord(p);
        QRgb pixel = texture.pixel(
            tc.x()*texture.width(), 
            tc.y()*texture.height());
        Vector3f color = { qRed(pixel) / 255.f, 
                           qGreen(pixel) / 255.f, 
                           qBlue(pixel) / 255.f };
        return color;
    }
    else {
        return material.ambient;
    }
}

void Object::loadTexture(const std::string& filename) {
    texture.load(filename.c_str());
    texture = texture.mirrored();
}

void Object::readMaterial(const YAML::Node& node) 
{
    Vector3f color = { 1, 1, 1 };
    if (node.FindValue("color"))
    {
        readVector3f(node["color"],color);
    }

    else if (node.FindValue("texture")) {
        string s; node["texture"] >> s;
        loadTexture(s);
    }
    float ka, kd, ks, n;
    node["ka"] >> ka;
    node["kd"] >> kd;
    node["ks"] >> ks;
    node["n"] >> n;
    material.ambient = ka*color;
    material.diffuse = kd*color;
    material.specular = ks*color;
    material.shine = n;
}

