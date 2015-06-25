#pragma once

#include <initializer_list>
#include <cassert>
#include <type_traits>
#include "yaml/yaml.h"
/*
    Vector<SIZE,T>
    N-Dimensional vector used for mathematical operations.
    Supports addition, subtraction, element-wise multiplication and division,
    scalar multiplication and division, and dot product.
    (Cross products and rotations around an arbitrary 3D axis are supported
    for 3-dimensional variant).
*/
template<int SIZE,typename T>
class Vector
{
public:
    Vector();    
    Vector(T value);
    Vector(const Vector<SIZE,T>& other);
    Vector(std::initializer_list<T> list);
    Vector(const Vector<SIZE - 1, T>& other);
    T* begin();
    T* end();
    double length();
    Vector<SIZE,T>& normalize();
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    Vector<SIZE, T>& operator=(const Vector<SIZE, T>& other);
    Vector<SIZE, T>& operator=(std::initializer_list<T> list);
    Vector<SIZE, T>& operator+=(const Vector<SIZE, T>& other);
    Vector<SIZE, T>& operator-=(const Vector<SIZE, T>& other);
    Vector<SIZE, T>& operator*=(const Vector<SIZE, T>& other);
    Vector<SIZE, T>& operator/=(const Vector<SIZE, T>& other);
    template<typename S> Vector<SIZE, T>& operator*=(S scalar);
    template<typename S> Vector<SIZE, T>& operator/=(S scalar);
public:    
    T& x() { return data_[0]; }
    template< class = std::enable_if<(SIZE >= 2)>::type>
    T& y() { return data_[1]; }
    template< class = std::enable_if<(SIZE >= 3)>::type>
    T& z() { return data_[2]; }
    template< class = std::enable_if<(SIZE >= 4)>::type>
    T& w() { return data_[3]; }
private:
    T data_[SIZE];
};

typedef Vector<2, float> Vector2f;
typedef Vector<3,float> Vector3f;
typedef Vector<4, float> Vector4f;


void readVector3f(const YAML::Node& node, Vector3f& v) 
{
    node[0] >> v[0];
    node[1] >> v[1];
    node[2] >> v[2];
}

#include "Vector.inl"
