#pragma once
#include "Vector3f.h"
#include "Ray.h"
/*
    3D Camera
    Class to represent a virtual camera in the 3D space for computer graphics.
    Keeps a camera position and the camera coordinate system, and supplies
    operations for rotating and translating the camera along these axes.
    Supports also translation around a fixed center of view, thus storing a
    specified center of view (lookAt).
*/

namespace YAML { class Node; }

class Camera
{
public:
    Camera();
    void read(const YAML::Node& node ,bool eye = false);
    void reset();
    void translateU(float amount, bool fixed_center = false);
    void translateV(float amount, bool fixed_center = false);
    void translateN(float amount, bool fixed_center = false);
    void rotateU(float angle);
    void rotateV(float angle);
    void rotateN(float angle);
    Vector3f position() const { return position_; }
    void position(const Vector3f& new_position)  { position_ = new_position; }
    Vector3f lookAt() const { return look_at_; }
    const Vector3f& u() const {return u_;};
    const Vector3f& v() const { return v_; };
    const Vector3f& n() const { return n_; };
    void resetLookAt(Vector3f Vector3f) { reset_look_at_ = Vector3f; }
    void resetPosition(Vector3f Vector3f) { reset_position_ = Vector3f; }
    Vector3f resetPosition() { return reset_position_; }
    Ray getRay(double x, double y) const;
private:
    double view_width;
    double view_height;
    Vector3f position_;
    Vector3f u_;
    Vector3f v_;
    Vector3f n_;
    Vector3f look_at_;
    Vector3f reset_look_at_;
    Vector3f reset_position_;
};