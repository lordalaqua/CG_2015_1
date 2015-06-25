#include "Sphere.h"
#include "yaml/yaml.h"
#include "Hit.h"
#include "Material.h"
#include <cmath>
#include <algorithm>
#include <ciso646>

const double PI = 3.14159265358979323846;
inline double degToRad(double degree) { return degree * PI / 180.0; }

inline double radToDeg(double rad) { return rad * 180.0 / PI; }

Sphere::Sphere(const YAML::Node& node)
: Object(node) {
    readVector3f(node["position"],position);
    node["radius"] >> r;
    double angle = 0;
    Vector3f axis = { 0, 1, 0 };
    if (node.FindValue("rotationAxis"))
        readVector3f(node["rotationAxis"], axis);
    if (node.FindValue("rotationAngle"))
        node["rotationAngle"] >> angle;

    angle = degToRad(angle);
    axis.normalize();

    u = axis.x(), v = axis.y(), w = axis.z();
    a = position.x(), b = position.y(), c = position.z();

    double vv = v * v, uu = u * u, ww = w * w;

    cosT = cos(angle), sinT = sin(angle);

    au = a * u, cw = c * w, cu = c * u, bv = b * v;
    cv = c * v, bw = b * w, aw = a * w, bu = b * u, av = a * v;
    aVVWW = a * (vv + ww), bUUWW = b * (uu + ww), cUUVV = c * (uu + vv);
}

Hit Sphere::intersect(const Ray &ray) const 
{
    const Vector3f& D = ray.getDirection();
    Vector3f OC = (ray.getOrigin() - position);
    double A = dotProduct(D, D);
    double B = 2 * dotProduct(OC, D);
    double C = dotProduct(OC, OC) - r * r;

    double disc = B * B - 4 * A * C;

    if (disc < 0.0) return Hit::None();
    disc = sqrt(disc);

    double t0 = (-B - disc) / (2 * A);
    double t1 = (-B + disc) / (2 * A);

    if (t1 < 0 and t0 < 0) return Hit::None();
    if (t0 > t1) std::swap(t0, t1);
    if (t0 < 0) return Hit::None();

    return Hit(this, t0, (ray.at(t0) - position).normalize());
}

Vector2f Sphere::getTextureCoord(Vector3f p) const 
{
    Vector3f vp = (p - position) * r;
    float phi = acosf(-dotProduct(vp, m_Vn));
    float u, v = phi * m_Material.GetVScaleReci() * (1.0f / PI);
    float theta = (acosf(dotProduct(m_Ve, vp) / sinf(phi))) * (0.5f / PI);
    if (dotProduct(m_Vc, vp) >= 0) u = (1.0f - theta) * m_Material.GetUScaleReci();
    else u = theta * m_Material.GetUScaleReci();

    Vector2f tc = { u, v };
    return tc;
}
