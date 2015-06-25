#pragma once
#include <ciso646>
#include "Vector.h"
#include "Material.h"
#include "Hit.h"
#include "Ray.h"
#include <QImage>

namespace YAML { class Node; }

class Object {
public:
    Object();

    Object(const YAML::Node& node);    

    virtual ~Object();

    virtual Hit intersect(const Ray &ray) const = 0;

    Vector3f colorAt(const Vector3f& p) const;

    bool isTextured() const { return not texture.isNull(); }

    void readMaterial(const YAML::Node& node);

    const Material& getMaterial() const { return material; }

private:

    void loadTexture(const std::string& filename);

    virtual Vector2f getTextureCoord(Vector3f Vector3f) const { return Vector2f(); }

    QImage texture;
    Material material;
};
