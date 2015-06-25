#include "Camera.h"
#include "yaml/yaml.h"

Camera::Camera()
{
    reset_position_ = { 0, 0, 0 };
    reset_look_at_ = { 0, 0, 1 };
    reset();
}

void Camera::read(const YAML::Node& node, bool eye /*= false*/)
{
    readVector3f(node["eye"],position_);
    readVector3f(node["up"], v_);
    readVector3f(node["center"],look_at_);
    n_ = (look_at_ - position_).normalize();
    u_ = crossProduct(v_, n_);
    reset_position_ = position_;
    reset_look_at_ = look_at_;

    auto& vsNode = node["viewSize"];
    vsNode[0] >> view_width;
    vsNode[1] >> view_height;
}

void Camera::reset()
{
    position_ = reset_position_;
    u_ = { 1, 0, 0 };
    v_ = { 0, 1, 0 };
    n_ = { 0, 0, 1 };
    look_at_ = reset_look_at_;
}

void Camera::translateU(float amount, bool fixed_center/*=false*/)
{
    if (fixed_center)
    {
        position_ = position_ + amount*u_;
        n_ = -(look_at_ - position_).normalize();
        u_ = crossProduct(v_, n_).normalize();
    }
    else
    {
        position_ += amount*u_;
        look_at_ += amount*u_;
    }
}

void Camera::translateV(float amount, bool fixed_center/*=false*/)
{
    if (fixed_center)
    {
        position_ = position_ + amount*v_;
        n_ = -(look_at_ - position_).normalize();
        v_ = crossProduct(n_,u_).normalize();
    }
    else
    {
        position_ += amount*v_;
        look_at_ += amount*v_;
    }
}

void Camera::translateN(float amount, bool fixed_center/*=false*/)
{
    position_ += amount*n_;
    look_at_ += amount*n_;
}

void Camera::rotateU(float angle)
{
    v_ = rotate(v_, u_, angle);
    n_ = rotate(n_, u_, angle);
    look_at_ = rotate(look_at_ - position_, u_, angle) + position_;
}

void Camera::rotateV(float angle)
{
    u_ = rotate(u_, v_, angle);
    n_ = rotate(n_, v_, angle);
    look_at_ = rotate(look_at_ - position_, v_, angle) + position_;
}

void Camera::rotateN(float angle)
{
    u_ = rotate(u_, n_, angle);
    v_ = rotate(v_, n_, angle);
}

Ray Camera::getRay(double x, double y) const
{
    Vector3f E = position_;
    Vector3f G = n_;
    Vector3f U = v_;
    double w = view_width;
    double h = view_height;

    Vector3f A = crossProduct(G, U);
    Vector3f B = crossProduct(A, G);
    Vector3f M = look_at_;
    Vector3f H = A * 0.5 * w / A.length();
    Vector3f V = B * 0.5 * h / B.length();
    double sx = x / view_width;
    double sy = y / view_height;

    Vector3f P = M + (2 * sx - 1) * H + (2 * sy - 1) * V;

    return Ray(E, (P - E).normalize());
}
