//#include "Vector.h"
#ifndef VECTOR_inl__
#define VECTOR_inl__

#include <iostream>
#include <cmath>
/************************** Member Functions **********************************/
template<int SIZE, typename T>
Vector<SIZE, T>::Vector()
{
    for (int i = 0; i < SIZE; ++i)
        data_[i] = 0;
}

template<int SIZE, typename T>
Vector<SIZE, T>::Vector(T value) : Vector()
{
    for (int i = 0; i < SIZE; ++i)
        data_[i] = value;
}

template<int SIZE, typename T>
Vector<SIZE, T>::Vector(const Vector<SIZE, T>& other) : Vector()
{
    *this = other;
}

template<int SIZE, typename T>
Vector<SIZE, T>::Vector(std::initializer_list<T> list) : Vector()
{
    *this = list;
}

template<int SIZE, typename T>
Vector<SIZE, T>::Vector(const Vector<SIZE - 1, T>& other) : Vector()
{
    for (int i = 0; i < SIZE - 1; ++i)
        data_[i] = other[i];
    data_[SIZE - 1] = 1;
}

template<int SIZE, typename T>
T* Vector<SIZE, T>::begin()
{
    return &data_[0];
}

template<int SIZE, typename T>
T* Vector<SIZE, T>::end()
{
    return &data_[SIZE];
}

template<int SIZE, typename T>
double Vector<SIZE, T>::length()
{
    double square_sum = 0;
    for (const T& element : data_)
        square_sum += (element*element);
    return sqrt(square_sum);
}

template<int SIZE, typename T>
Vector<SIZE, T>& Vector<SIZE, T>::normalize()
{
    double prev_length = length();
    for (T& element : data_)
        element = static_cast<T>(static_cast<double>(element) / prev_length);
    return *this;
}

template<int SIZE, typename T>
T& Vector<SIZE, T>::operator[](size_t index)
{
    return data_[index];
}

template<int SIZE, typename T>
const T& Vector<SIZE, T>::operator[](size_t index) const
{
    return data_[index];
}

template<int SIZE, typename T>
Vector<SIZE, T>& Vector<SIZE, T>::operator=(const Vector<SIZE, T>& other)
{
    for (int i = 0; i < SIZE; ++i)
        data_[i] = other.data_[i];
    return *this;
}

template<int SIZE, typename T>
Vector<SIZE, T>& Vector<SIZE,T>::operator=(std::initializer_list<T> list)
{
    if (list.size() != SIZE)
        throw 1; // TODO: proper exception type
    int i = 0;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        data_[i] = *it;
        ++i;
    }
    return *this;
}

template<int SIZE, typename T>
Vector<SIZE, T>& Vector<SIZE, T>::operator+=(const Vector<SIZE, T>& other)
{
    for (int i = 0; i < SIZE; ++i)
        data_[i] += other.data_[i];
    return *this;
}

template<int SIZE, typename T>
Vector<SIZE, T>& Vector<SIZE, T>::operator-=(const Vector<SIZE, T>& other)
{
    for (int i = 0; i < SIZE; ++i)
        data_[i] -= other.data_[i];
    return *this;
}
template<int SIZE, typename T>
Vector<SIZE, T>& Vector<SIZE,T>::operator*=(const Vector<SIZE, T>& other)
{
    for (int i = 0; i < SIZE; ++i)
        data_[i] *= other.data_[i];
    return *this;
}
template<int SIZE, typename T>
Vector<SIZE, T>& Vector<SIZE, T>::operator/=(const Vector<SIZE, T>& other)
{
    for (int i = 0; i < SIZE; ++i)
        data_[i] /= other.data_[i];
    return *this;
}

template<int SIZE, typename T>
template<typename S>
Vector<SIZE,T>& Vector<SIZE,T>::operator*=(S scalar)
{
    T cast = static_cast<T>(scalar);
    for (T& element : data_)
        element *= cast;
    return *this;
}

template<int SIZE, typename T>
template<typename S>
Vector<SIZE,T>& Vector<SIZE,T>::operator/=(S scalar)
{
    T cast = static_cast<T>(scalar);
    for (T& element : data_)
        element /= cast;
    return *this;
}


/***************************** Non-member functions ***************************/
template<int SIZE, typename T>
Vector<SIZE,T> getNormalized(Vector<SIZE,T> vector)
{
    return vector.normalize();
}

template<int SIZE, typename T>
bool operator==(const Vector<SIZE,T>& first, const Vector<SIZE,T>& second)
{
    bool is_equal = true;
    for (int i = 0; i < SIZE; ++i)
        is_equal = is_equal && (first[i] == second[i]);
    return is_equal;
}

template<int SIZE, typename T>
bool operator!=(const Vector<SIZE,T>& first, const Vector<SIZE,T>& second)
{
    return !(first == second);
}

template<int SIZE, typename T>
Vector<SIZE,T> operator+(Vector<SIZE,T> first, const Vector<SIZE,T>& second)
{
    return first += second;
}

template<int SIZE, typename T>
Vector<SIZE,T> operator-(Vector<SIZE,T> first, const Vector<SIZE,T>& second)
{
    return first -= second;
}

template<int SIZE, typename T>
Vector<SIZE,T> operator-(Vector<SIZE,T> vector)
{
    for (int i = 0; i < SIZE; ++i)
        vector[i] = -vector[i];
    return vector;
}

template<int SIZE, typename T>
Vector<SIZE,T> operator*(Vector<SIZE,T> first, const Vector<SIZE,T>& second)
{
    return first *= second;
}

template<int SIZE, typename T>
Vector<SIZE,T> operator/(Vector<SIZE,T> first, const Vector<SIZE,T>& second)
{
    return first /= second;
}

template<int SIZE, typename T, typename S>
Vector<SIZE,T> operator*(Vector<SIZE,T> vector, S scalar)
{
    return vector *= scalar;
}

template<int SIZE, typename T, typename S>
Vector<SIZE,T> operator*(S scalar, Vector<SIZE,T> vector)
{
    return vector *= scalar;
}

template<int SIZE, typename T, typename S>
Vector<SIZE,T> operator/(Vector<SIZE,T> vector, S scalar)
{
    return vector /= scalar;
}

template<int SIZE, typename T>
T dotProduct(const Vector<SIZE,T>& first, const Vector<SIZE,T>& second)
{
    Vector<SIZE,T> product = first * second;
    T sum = 0;
    for (const T& element : product)
        sum += element;
    return sum;
}

template<typename T>
Vector<3,T> crossProduct(Vector<3,T> first, const Vector<3,T>& second)
{
    Vector<3,T> result;
    result[0] = first[1]*second[2] - first[2]*second[1];
    result[1] = first[2]*second[0] - first[0]*second[2];
    result[2] = first[0]*second[1] - first[1]*second[0];
    return result;
}


template<typename T>
Vector<3,T> rotate(Vector<3,T> v, Vector<3,T> axis, int degrees)
{
    Vector<3,T> result;
    double theta = degrees*(3.14159265 / 180);
    Vector<3,T> q = getNormalized(axis)*sin(theta / 2);
    double q0 = cos(theta / 2), q1 = q[0], q2 = q[1], q3 = q[2];

    result[0] = static_cast<T>((1 - 2 * (q2*q2 + q3*q3))*v[0] + 2 * (q1*q2 - q0*q3)*v[1] + 2 * (q1*q3 + q0*q2)*v[2]);
    result[1] = static_cast<T>(2 * (q1*q2 + q0*q3)*v[0] + (1 - 2 * (q1*q1 + q3*q3))*v[1] + 2 * (q2*q3 - q0*q1)*v[2]);
    result[2] = static_cast<T>(2 * (q1*q3 - q0*q2)*v[0] + (2 * (q2*q3 + q0*q1))*v[1] + (1 - 2 * (q1*q1 - q2*q2))*v[2]);
    return result;
}

template<int SIZE, typename T>
std::ostream& operator<<(std::ostream& stream, const Vector<SIZE,T>& vector)
{
    stream << "["; 
    for (int i = 0; i < SIZE; ++i)
    {
        stream << vector[i];
        if (i < SIZE - 1)
            stream << ", ";
    }
    stream << "]";
    return stream;
}

#endif // Vector3D_h__

