//#include "Vector3D.hpp"
#ifndef Vector3D_inl__
#define Vector3D_inl__

#include <iostream>
#include <cmath>
/************************** Member Functions **********************************/
template<typename T>
Vector3D<T>::Vector3D(std::initializer_list<T> list)
{
    assert(list.size() == 3);
    auto it = list.begin();
    x = *it++;
    y = *it++;
    z = *it;
}

template<typename T>
double Vector3D<T>::length()
{
    double square_sum = x*x + y*y + z*z;
    return sqrt(square_sum);
}

template<typename T>
Vector3D<T>& Vector3D<T>::normalize()
{
    double prev_length = length();
    x = static_cast<T>(static_cast<double>(x)/ prev_length);
    y = static_cast<T>(static_cast<double>(y) / prev_length);
    z = static_cast<T>(static_cast<double>(z) / prev_length);
    return *this;
}


template<typename T>
Vector3D<T>& Vector3D<T>::operator=(const Vector3D<T>& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

template<typename T>
Vector3D<T>& Vector3D<T>::operator+=(const Vector3D<T>& other)
{
    x = x + other.x;
    y = y + other.y;
    z = z + other.z;
    return *this;
}

template<typename T>
Vector3D<T>& Vector3D<T>::operator-=(const Vector3D<T>& other)
{
    x = x - other.x;
    y = y - other.y;
    z = z - other.z;
    return *this;
}

template<typename T>
Vector3D<T>& Vector3D<T>::operator*=(const Vector3D<T>& other)
{
    x = x * other.x;
    y = y * other.y;
    z = z * other.z;
    return *this;
}

template<typename T>
Vector3D<T>& Vector3D<T>::operator/=(const Vector3D<T>& other)
{
    assert(other.x != 0 && other.y != 0);
    x = x / other.x;
    y = y / other.y;
    z = z / other.z;
    return *this;
}

template<typename T>
template<typename S>
Vector3D<T>& Vector3D<T>::operator*=(S scalar)
{
    T cast = static_cast<T>(scalar);
    x = x * cast;
    y = y * cast;
    z = z * cast;
    return *this;
}

template<typename T> 
template<typename S>
Vector3D<T>& Vector3D<T>::operator/=(S scalar)
{
    assert(scalar != 0);
    T cast = static_cast<T>(scalar);
    x = x / cast;
    y = y / cast;
    z = z / cast;
    return *this;
}

/***************************** Non-member functions ***************************/
template<typename T>
Vector3D<T> getNormalized(Vector3D<T> vector)
{
    return vector.normalize();
}
template<typename T>
bool operator==(const Vector3D<T>& first, const Vector3D<T>& second)
{
    return first.x == second.x && first.y == second.y && first.z == second.z;
}

template<typename T>
bool operator!=(const Vector3D<T>& first, const Vector3D<T>& second)
{
    return !(first == second);
}

template<typename T>
Vector3D<T> operator+(Vector3D<T> first, const Vector3D<T>& second)
{
    return first += second;
}

template<typename T>
Vector3D<T> operator-(Vector3D<T> first, const Vector3D<T>& second)
{
    return first -= second;
}

template<typename T>
Vector3D<T> operator-(Vector3D<T> vector)
{
    return Vector3D<T>{-vector.x,-vector.y,-vector.z};
}

template<typename T>
Vector3D<T> operator*(Vector3D<T> first, const Vector3D<T>& second)
{
    return first *= second;
}

template<typename T>
Vector3D<T> operator/(Vector3D<T> first, const Vector3D<T>& second)
{
    return first /= second;
}

template<typename T, typename S>
Vector3D<T> operator*(Vector3D<T> vector, S scalar)
{
    return vector *= scalar;
}

template<typename T, typename S>
Vector3D<T> operator*(S scalar, Vector3D<T> vector)
{
    return vector *= scalar;
}

template<typename T, typename S>
Vector3D<T> operator/(Vector3D<T> vector, S scalar)
{
    return vector /= scalar;
}

template<typename T>
T dotProduct(Vector3D<T> first, const Vector3D<T>& second)
{
    Vector3D<T> product = first * second;
    return product.x + product.y + product.z;
}

template<typename T>
Vector3D<T> crossProduct(Vector3D<T> first, const Vector3D<T>& second)
{
    Vector3D<T> result;
    result.x = first.y*second.z - first.z*second.y;
    result.y = first.z*second.x - first.x*second.z;
    result.z = first.x*second.y - first.y*second.x;
    return result;
}


template<typename T>
Vector3D<T> rotate(Vector3D<T> v, Vector3D<T> axis, int degrees)
{
    Vector3D<T> result;
    double theta = degrees*(3.14159265 / 180);
    Vector3D<T> q = getNormalized(axis)*sin(theta/2);
    float q0 = cos(theta / 2), q1 = q.x, q2 = q.y, q3 = q.z;

    result.x = (1 - 2 * (q2*q2 + q3*q3))*v.x + 2 * (q1*q2 - q0*q3)*v.y + 2 * (q1*q3 + q0*q2)*v.z;
    result.y = 2 * (q1*q2 + q0*q3)*v.x + (1 - 2 * (q1*q1 + q3*q3))*v.y + 2 * (q2*q3 - q0*q1)*v.z;
    result.z = 2 * (q1*q3 - q0*q2)*v.x + (2 * (q2*q3 + q0*q1))*v.y + (1- 2 * (q1*q1 - q2*q2))*v.z;
    return result;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Vector3D<T>& vector) 
{
    stream << "[" << vector.x << "," << vector.y << "," << vector.z << "]";
    return stream;
}

#endif // Vector3D_h__