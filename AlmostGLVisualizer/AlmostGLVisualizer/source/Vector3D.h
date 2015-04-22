#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

#include <initializer_list>
/*
    Vector3D
    3-Dimensional vector used for mathematical operations.
    Supports addition, subtraction, element-wise multiplication and division,
    scalar multiplication and division, dot and cross products and rotations
    around an arbitrary 3D axis.

    TODO: resolve replication between arbitrary Vector<SIZE,type> and this.
*/
template<typename T>
class Vector3D
{
public:
    Vector3D() : x(), y(), z() {}
    Vector3D(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    Vector3D(T V) : x(V), y(V), z(V) {}
    Vector3D(const Vector3D<T>& other) : x(other.x), y(other.y), z(other.z) {}
    Vector3D(std::initializer_list<T> list);
    ~Vector3D() {};

    double length();
    Vector3D<T>& normalize();

    Vector3D<T>& operator=(const Vector3D<T>& other);
    Vector3D<T>& operator+=(const Vector3D<T>& other);
    Vector3D<T>& operator-=(const Vector3D<T>& other);
    Vector3D<T>& operator*=(const Vector3D<T>& other);
    Vector3D<T>& operator/=(const Vector3D<T>& other);
    template<typename S> Vector3D<T>& operator*=(S scalar);
    template<typename S> Vector3D<T>& operator/=(S scalar);
public:
    T x, y, z;
};

typedef Vector3D<int> Vector3i;
typedef Vector3D<float> Vector3f;
typedef Vector3D<double> Vector3d;

#include "Vector3D.inl"
#endif