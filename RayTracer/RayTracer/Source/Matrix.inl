#ifndef Matrix_inl__
#define Matrix_inl__

#include "Matrix.h"

template<int ROWS, int COLS, typename T>
Matrix<ROWS, COLS, T>::Matrix(std::initializer_list<std::initializer_list<T>> list)
{
    *this = list;
}

template<int ROWS, int COLS, typename T>
Matrix<ROWS, COLS, T>::Matrix(const Matrix<ROWS, COLS, T>& other)
{
    *this = other;
}

template<int ROWS, int COLS, typename T>
Matrix<COLS, ROWS, T> Matrix<ROWS, COLS, T>::transposed() const
{
    Matrix<COLS, ROWS, T> result;
    for (int i = 0; i < ROWS; ++i)
    for (int j = 0; j < COLS; ++j)
        result[j][i] = data_[i][j];
    return result;
}

template<int ROWS, int COLS, typename T>
Vector<COLS, T>& Matrix<ROWS, COLS, T>::operator[](size_t index)
{
    return data_[index];
}

template<int ROWS, int COLS, typename T>
const Vector<COLS, T>& Matrix<ROWS, COLS, T>::operator[](size_t index) const
{
    return data_[index];
}

template<int ROWS, int COLS, typename T>
T& Matrix<ROWS, COLS, T>::operator()(size_t row, size_t col)
{
    return data_[row][col];
}

template<int ROWS, int COLS, typename T>
const T& Matrix<ROWS, COLS, T>::operator()(size_t row, size_t col) const
{
    return data_[row][col];
}

template<int ROWS, int COLS, typename T>
Matrix<ROWS, COLS, T>& Matrix<ROWS, COLS, T>::operator=(const Matrix<ROWS, COLS, T>& other)
{
    data_ = other.data_;
    return *this;
}

template<int ROWS, int COLS, typename T>
Matrix<ROWS, COLS, T>& Matrix<ROWS, COLS, T>::operator=(std::initializer_list<std::initializer_list<T>> list)
{
    if (list.size() != ROWS)
        throw 1; // TODO: exception type
    int i = 0;
    for (auto it_row = begin(list); it_row != end(list); ++it_row)
    {
        data_[i] = *it_row; // Construct matrix, each row is a Vector
        ++i;
    }
    return *this;
}


template<int ROWS, int COLS, typename T>
bool operator==(const Matrix<ROWS, COLS, T>& first, const Matrix<ROWS, COLS, T>& second)
{
    bool is_equal = true;
    for (int i = 0; i < ROWS; ++i)
        is_equal = is_equal && (first[i] == second[i]);
    return is_equal;

}

template<int ROWS, int COLS, typename T>
Matrix<ROWS, COLS, T> operator+(const Matrix<ROWS, COLS, T>& first, const Matrix<ROWS, COLS, T>& second)
{
    Matrix<ROWS, COLS, T> result;
    result.data_ = first.data_ + second.data_;
    return result;
}

template<int ROWS, int COLS, typename T>
Matrix<ROWS, COLS, T> operator-(const Matrix<ROWS, COLS, T>& first, const Matrix<ROWS, COLS, T>& second)
{
    Matrix<ROWS, COLS, T> result;
    result.data_ = first.data_ - second.data_;
    return result;
}

template<int ROWS, int COLS, typename T>
Matrix<ROWS, COLS, T> operator-(const Matrix<ROWS, COLS, T>& matrix)
{
    Matrix<ROWS, COLS, T> result;
    result.data_ = -matrix.data_;
    return result;
}

template<int ROWS, int COMMON, int COLS, typename T>
Matrix<ROWS, COLS, T> operator*(const Matrix<ROWS, COMMON, T>& first, const Matrix<COMMON, COLS, T>& second)
{
    Matrix<ROWS, COLS, T> result;
    Matrix<COLS, COMMON, T> transposed = second.transposed();
    for (int i = 0; i < ROWS; ++i)
    for (int j = 0; j < COLS; ++j)
        result[i][j] = dotProduct(first[i], transposed[j]);
    return result;
}

template<int ROWS, int COLS, typename T>
Vector<ROWS, T> operator*(const Matrix<ROWS, COLS, T>& matrix, const Vector<COLS, T>& vector)
{
    Vector<ROWS, T> result;
    for (int i = 0; i < ROWS; ++i)
        result[i] = dotProduct(matrix[i], vector);
    return result;
}

template<int ROWS, int COLS, typename T>
std::ostream& operator<<(std::ostream& stream, const Matrix<ROWS, COLS, T>& matrix)
{
    stream << matrix.data_;
    return stream;
}

#endif // Matrix_inl__
