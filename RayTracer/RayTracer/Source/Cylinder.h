#pragma once
#include "Object.h"
#include "Vector.h"

namespace YAML { class Node; }

class Cylinder : public Object {
public:
	Cylinder(const YAML::Node& node);

	Hit intersect(const Ray &ray) const;

private:
	Vector3f top, bottom;
	double radius;
};
