#include "Camera.h"

Camera::Camera()
{
    reset_look_at_ = { 0, 0, 1 };
    reset();
}

void Camera::reset()
{
    position_ = reset_position_;
    u_ = { 1, 0, 0 };
    v_ = { 0, 1, 0 };
    n_ = { 0, 0, 1 };
    h_fov_ = 60;
    v_fov_ = 60;
    z_near_ = -1;
    z_far_ = 10000;
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
