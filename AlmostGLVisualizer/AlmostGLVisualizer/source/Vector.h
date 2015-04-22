#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <initializer_list>
#include <cassert>
#include "Vector3D.h"

/*
    Vector<SIZE,T>
    N-Dimensional vector used for mathematical operations.
    Supports addition, subtraction, element-wise multiplication and division,
    scalar multiplication and division, and dot product.
    (Cross products and rotations around an arbitrary 3D axis are supported
    for 3-dimensional variant).

    TODO: resolve replication between arbitrary Vector3D and this.
*/
template<int SIZE,typename T>
class Vector
{
public:
    Vector(){}
    Vector(T value);
    Vector(const Vector<SIZE,T>& other);
    Vector(std::initializer_list<T> list);
    Vector(const Vector3D<T>& old) 
    {
        assert(SIZE == 4);
        data_[0] = old.x;
        data_[1] = old.y;
        data_[2] = old.z;
        data_[3] = 1;
    }
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
private:
    T data_[SIZE];
};

#include "Vector.inl"

#endif