#ifndef Camera_h__
#define Camera_h__

#include "Vector.h"

/*
    3D Camera
    Class to represent a virtual camera in the 3D space for computer graphics.
    Keeps a camera position and the camera coordinate system, and supplies
    operations for rotating and translating the camera along these axes.
    Supports also translation around a fixed center of view, thus storing a
    specified center of view (lookAt).
*/

class Camera
{
public:
    Camera();
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
    void Hfov(float angle) { h_fov_ = angle; }
    float Hfov() const { return h_fov_; }
    void Vfov(float angle) { v_fov_ = angle; }
    float Vfov() const { return v_fov_; }
    void Znear(float value) { z_near_ = value; }
    float Znear() const { return z_near_; }
    void Zfar(float value) { z_far_ = value; }
    float Zfar() const { return z_far_; }
    void setResetLookAt(Vector3f point) { reset_look_at_ = point; }
    void setResetPosition(Vector3f point) { reset_position_ = point; }
    Vector3f getResetPosition() { return reset_position_; }
private:
    Vector3f position_;
    Vector3f u_;
    Vector3f v_;
    Vector3f n_;
    Vector3f look_at_;
    Vector3f reset_look_at_;
    Vector3f reset_position_;
    float h_fov_;
    float v_fov_;
    float z_near_;
    float z_far_;
};


#endif // Camera_h__