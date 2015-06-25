#include "Torus.h"
#include "Hit.h"
#include "yaml/yaml.h"
#include <algorithm>
#include <ciso646>
#include <iostream>

using namespace std;

Torus::Torus(const YAML::Node& node) 
	: Object(node) {
	node["position"] >> position;
	node["bigRadius"] >> R;
	node["smallRadius"] >> r;
}

Hit Torus::intersect(const Ray& ray) const {
	Vector3f rayO = ray.getOrigin() - position;
	Vector3f rayD = ray.getDirection(); 
	double RR = R * R, rr = r * r;

	double alpha = rayD.dot(rayD);
	double beta = 2 * rayO.dot(rayD);
	double gamma = rayO.dot(rayO) - rr - RR;

	double a = alpha * alpha;
	double b = 2 * alpha * beta;
	/* Não entendi porque o d.z aqui.... */
	double c = beta * beta + 2 * alpha * gamma + 4 * RR * rayD.z * rayD.z;
	double d = 2 * beta * gamma + 8 * RR * rayO.z * rayD.z;
	double e = gamma * gamma + 4 * RR * rayO.z * rayO.z - 4 * RR * rr;

	double x1, x2, x3, x4;
	if (solveQuadricEquation(a, b, c, d, e, x1, x2, x3, x4) == false)
		return Hit::noHit();

	double t = INF;
	if (x1 == x1) t = min(t, x1);
	if (x2 == x2) t = min(t, x2);
	if (x3 == x3) t = min(t, x3);
	if (x4 == x4) t = min(t, x4);
	if (t == INF) return Hit::noHit();

	Vector3f p = rayO + t * rayD;
	double alex = 4 * (p.dot(p) - rr - RR);
	Vector3f N;
	N.x = p.x * alex;
	N.y = p.y * alex;
	N.z = p.z * alex + 8 * RR * p.z;

	p += position;

	double newT = INF;
	if (not isPracticallyZero(rayD.x)) newT = p.x - ray.getOrigin().x / rayD.x;
	else if (not isPracticallyZero(rayD.y)) newT = p.y - ray.getOrigin().y / rayD.y;
	else if (not isPracticallyZero(rayD.z)) newT = p.z - ray.getOrigin().z / rayD.z;
	else return Hit::noHit();

	return Hit(this, newT, N.normalized());
}
