#pragma once

#include <iostream>
#include "Vector3f.h"
#include "Object.h"

namespace YAML { class Node; }


class LightSource {
public:
    LightSource(const YAML::Node& node);

    LightSource(const Vector3f& position, const Vector3f& color)
        : position(position), color(color) { }

    void read(const YAML::Node& node);

    const Vector3f& getColor() const { return color; }
    void setColor(const Vector3f& val) { color = val; }

    const Vector3f& getPosition() const { return position; }
    void setPosition(const Vector3f& val) { position = val; }

private:
    Vector3f position;
    Vector3f color;
};
