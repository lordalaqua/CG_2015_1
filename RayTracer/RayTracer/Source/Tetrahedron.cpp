#include "Tetrahedron.h"
#include "Triangle.h"
#include "Hit.h"
#include <algorithm>
#include "yaml/yaml.h"

using namespace std;

Tetrahedron::Tetrahedron(const YAML::Node& node) 
	: Object(node) {
	node["v0"] >> v0;
	node["v1"] >> v1;
	node["v2"] >> v2;
	node["v3"] >> v3;
}

Hit Tetrahedron::intersect(const Ray &ray) const {
	Hit minHit, h;
	if (h = Triangle::intersect(this, ray, v0, v1, v2))
		minHit = min(minHit, h);
	if (h = Triangle::intersect(this, ray, v0, v2, v3))
		minHit = min(minHit, h);
	if (h = Triangle::intersect(this, ray, v0, v1, v3))
		minHit = min(minHit, h);
	if (h = Triangle::intersect(this, ray, v1, v2, v3))
		minHit = min(minHit, h);
	return minHit;
}
