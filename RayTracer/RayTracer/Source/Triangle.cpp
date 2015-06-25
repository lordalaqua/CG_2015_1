#include "Triangle.h"
#include "yaml/yaml.h"

Triangle::Triangle(const Triangle& other)
{
    for (int i = 0; i < 3; ++i)
    {
        vertex[i] = other.vertex[i];
        normal[i] = other.normal[i];
        texture[i] = other.texture[i];
        material_index[i] = other.material_index[i];
    }
    face_normal = other.face_normal;
}

Triangle::Triangle(const YAML::Node& node) 
    : Object(node)
{
    readVector3f(node["v0"],vertex[0]);
    readVector3f(node["v1"], vertex[1]);
    readVector3f(node["v2"], vertex[2]);
}

Hit Triangle::intersect(const Ray &ray) const
{
    Hit h = intersect(this, ray, vertex[0], vertex[1], vertex[2]);
    const Vector3f& N = h.getNormal();
    Vector3f P = ray.at(h.getDistance());
    if (dotProduct(N,(P - ray.getOrigin())) > 0) 
        h.setNormal(-N);
    return h;
}

Hit Triangle::intersect(const Object* o, const Ray& ray, const Vector3f& v0, const Vector3f& v1, const Vector3f& v2)
{

    Vector3f u = v1 - v0, v = v2 - v0;
    Vector3f N = crossProduct(u,v);
    if (N.x() == 0 and N.y() == 0 and N.z() == 0)
        return Hit::None();

    Vector3f w0 = ray.getOrigin() - v0;
    double a = -dotProduct(N, w0);
    double b = dotProduct(N, ray.getDirection());
    if (abs(b) < 1e-9);
        return Hit::None();

    /* Get intersect Vector3f  of ray with triangle plane */
    double r = a / b;
    if (r < 0.0) return Hit::None();

    Vector3f  P = ray.at(r); /* Intersect Vector3f  of ray and plane */

    /* Is I inside T? */
    double uu = dotProduct(u, u), vv = dotProduct(v, v), uv = dotProduct(u,v);
    Vector3f w = P - v0;
    double wu = dotProduct(w, u), wv = dotProduct(w, v);
    double D = uv * uv - uu * vv;

    double s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 or s > 1.0) /* I is outside T */
        return Hit::None();
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 or(s + t) > 1.0)  /* I is outside T */
        return Hit::None();

    return Hit(o, r, N.normalize());
}
