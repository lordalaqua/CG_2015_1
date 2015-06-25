#pragma once
#include "Object.h"
#include "Triple.h"

namespace YAML { class Node; }

class Tetrahedron : public Object {
public:
	Tetrahedron(const YAML::Node& node);

	Hit intersect(const Ray& ray) const;

private:
	Vector3f v0, v1, v2, v3;
};

