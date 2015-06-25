#include "Cylinder.h"
#include "Hit.h"
#include "Ray.h"
#include "yaml/yaml.h"
#include <algorithm>
#include <ciso646>
#include <vector>
#include <cmath>

using namespace std;

Cylinder::Cylinder(const YAML::Node& node) 
	: Object(node) {
	node["top"] >> top;
	node["bottom"] >> bottom;
	node["radius"] >> radius;
}

Hit Cylinder::intersect(Ray &ray) const {
	Vector3f& v = ray.getDirection().normalize();
	Vector3f& p = ray.getOrigin();
	Vector3f va = (top - bottom).normalize();
	Vector3f deltaP = p - bottom;
	Vector3f V1 = (v - va * v.dot(va));
	Vector3f V2 = (deltaP - (va * va.dot(deltaP)));
	double A = V1.dot(V1);
	double B = 2 * (V1.dot(V2));
	double r2 = radius * radius;
	double C = V2.dot(V2) - r2;

	double discriminant = B * B - 4 * A * C;

	/* disc < 0.0: line misses the sphere completely */
	if (discriminant < 0.0) return Hit::noHit();
	discriminant = sqrt(discriminant);

	double t0 = (-B - discriminant) / (2 * A);
	double t1 = (-B + discriminant) / (2 * A);
	Vector3f rayAtT0 = ray.at(t0);
	Vector3f rayAtT1 = ray.at(t1);

	double minT = INF;

	if (t0 > 0 and dot(va, rayAtT0 - bottom) > 0 and dot(va, rayAtT0 - top) < 0)
		minT = min(minT, t0);
	if (t1 > 0 and dot(va, rayAtT1 - bottom) > 0 and dot(va, rayAtT1 - top) < 0)
		minT = min(minT, t1);

	bool inCap = false;

	double vDotVa = dot(v, va);
	if (vDotVa != 0/*not isPracticallyZero(vDotVa)*/) {
		double t = minT;
		double t2 = -va.dot(p - bottom) / vDotVa;
		double t3 = -va.dot(p - top) / vDotVa;
		Vector3f q3 = p + v * t3, q2 = p + v * t2;
		if (t2 > 0 and(q2 - bottom).dot(q2 - bottom) < r2)
			t = min(t, t2);
		if (t3 > 0 and(q3 - top).dot(q3 - top) < r2)
			t = min(t, t3);
		if (t < minT) {
			minT = t;
			inCap = true;
		}
	}
	if (minT == INF or minT < 0) {
		return Hit::noHit();
	}

	Vector3f hit = ray.at(minT);
	Vector3f a = hit - bottom;
	Vector3f b = va * dot(a, va);
	Vector3f N;
	if (inCap) {
		double distTop = dot(top - hit, top - hit);
		double distBottom = dot(bottom - hit, bottom - hit);
		if (distTop < distBottom) {
			N = (top - bottom).normalized();
		} else {
			N = (bottom - top).normalized();
		}
	} else {
		N = (a - b).normalized();
	}
	return Hit(this, minT, N);
}

