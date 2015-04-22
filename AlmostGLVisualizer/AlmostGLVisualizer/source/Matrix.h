#ifndef Matrix_h__
#define Matrix_h__

#include "Vector.h"
#include <initializer_list>
#include <iostream>

/*
    NxN Matrix
    Arbitrary dimension size, two-dimensional matrix.

    Used for mathematical operations, composed of a Vector of Vectors.
    Supports transposition, addition, subtraction and multiplication 
    by another matrix and by a Vector (considering size constraints).
    
    Something useful is support of construction by initializer_list
    in the format
    Matrix M = {
        {m00,m01,m02,...},
        {m10,m11,m12,...},
        {m20,m21,m22,...},
        ... };
*/

template<int ROWS, int COLS, typename T>
class Matrix
{
public:
    Matrix() {};
    Matrix(std::initializer_list<std::initializer_list<T>> list);
    Matrix(const Matrix<ROWS, COLS, T>& other);

    Matrix<COLS,ROWS,T> transposed() const;

    Vector<COLS, T>& operator[](size_t index);
    const Vector<COLS, T>& operator[](size_t index) const;
    T& operator()(size_t row, size_t col);
    const T& operator()(size_t row, size_t col) const;

    Matrix<ROWS, COLS, T>& operator=(const Matrix<ROWS, COLS, T>& other);
    Matrix<ROWS, COLS, T>& operator=(std::initializer_list<std::initializer_list<T>> list);

    template<int ROWS, int COLS, typename T>
    friend Matrix<ROWS, COLS, T> operator+(const Matrix<ROWS, COLS, T>& first, const Matrix<ROWS, COLS, T>& second);

    template<int ROWS, int COLS, typename T>
    friend Matrix<ROWS, COLS, T> operator-(const Matrix<ROWS, COLS, T>& first, const Matrix<ROWS, COLS, T>& second);

    template<int ROWS, int COLS, typename T>
    friend Matrix<ROWS, COLS, T> operator-(const Matrix<ROWS, COLS, T>& matrix);

    template<int ROWS, int COLS, typename T>
    friend std::ostream& operator<<(std::ostream& stream, const Matrix<ROWS, COLS, T>& matrix);
private:
    Vector<ROWS, Vector<COLS, T>> data_;
};

#include "Matrix.inl"


typedef Matrix<3, 3, float> Matrix3x3f;
typedef Matrix<4, 4, float> Matrix4x4f;

#endif // Matrix_h__
