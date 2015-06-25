#pragma once

#include "Object.h"

namespace YAML { class Node; }

class Torus : public Object {
public:
	Torus(const YAML::Node& node);

	Hit intersect(const Ray& ray) const;

private:
	double R, r;
	Vector3f position;
};

