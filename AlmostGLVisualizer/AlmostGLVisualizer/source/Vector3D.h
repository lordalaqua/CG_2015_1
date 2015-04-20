#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

#include <initializer_list>
#include <cassert>
#include <cmath>

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
    Vector3D& normalize();

    Vector3D& operator=(const Vector3D& other);
    Vector3D& operator+=(const Vector3D& other);
    Vector3D& operator-=(const Vector3D& other);
    Vector3D& operator*=(const Vector3D& other);
    Vector3D& operator/=(const Vector3D& other);
    template<typename S> Vector3D& operator*=(S scalar);
    template<typename S> Vector3D& operator/=(S scalar);
public:
    T x, y, z;
};

typedef Vector3D<int> Vector3i;
typedef Vector3D<float> Vector3f;
typedef Vector3D<double> Vector3d;

#include "Vector3D.inl"
#endif