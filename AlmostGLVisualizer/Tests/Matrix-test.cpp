#include <UnitTest++.h>
#include "Matrix.h"
TEST(MatrixInitialization)
{
    Matrix<3, 3, int> m({ { 1, 2, 3 }, { 1, 2, 3 }, { 2, 2, 3 } });
    m[0][1] = 4;
    CHECK(m[0][0] == 1);
    CHECK(m[0][1] == 4);
    CHECK(m[0][2] == 3);
    m(0, 2) = 10;
    CHECK(m[0][2] == 10);
    CHECK(m(1,1) == 2);
    CHECK(m(2, 2) == 3);
    Matrix<2, 4, int> m2 = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 } };
    m2(1, 3) = 9;
    m2[1][2] = 3;
    CHECK(m2[0][1] == 2);
    CHECK(m2[1][1] == 6);
    CHECK(m2[1][3] == 9);
    CHECK(m2(1, 2) == 3);
}

TEST(MatrixTranspose)
{
    Matrix<2, 3, int> m1 = { { 1, 2, 3 }, { 4, 5, 6 } };
    auto m2 = m1.transposed();
    Matrix<3, 2, int> m3 = { { 1, 4 }, { 2, 5 }, { 3, 6 } };
    Vector<2, Vector<2, int>> v = { { 1, 2 }, { 3, 4 } };
    CHECK(m2 == m3);
    CHECK(m1 == m2.transposed());
}

TEST(MatrixAddSubtract)
{
    Matrix<2, 2, float> m1 = { { 1, 2}, { 4, 5 } };
    Matrix<2, 2, float> m2 = { { 6, 7 }, { 8, 9 } };
    Matrix<2, 2, float> add = { { 7, 9 }, { 12, 14 } };
    Matrix<2, 2, float> zero = { { 0, 0 }, { 0, 0 } };
    Matrix<2, 2, float> sub = { { 5, 5 }, { 4, 4 } };
    Matrix<2, 2, float> sub2 = { { -5, -5 }, { -4, -4 } };
    CHECK(add == m2 + m1);
    CHECK(sub == m2 - m1);
    CHECK(sub2 == m1 - m2);
    CHECK(sub == (-sub2));
    CHECK((zero == m1 - m1) && (zero == m2 - m2));
}

TEST(MatrixMultiplication)
{
    Matrix<2, 3, float> m1 = { { 1, 2, 3 }, { 4, 5, 6 } };
    Matrix<3, 2, float> m2 = { { 1, 4 }, { 2, 5 }, { 3, 6 } };
    Matrix<2, 2, float> i = { { 1, 0}, { 0, 1} };
    Matrix<2, 2, float> result = { { 14, 32 }, { 32, 77 }};
    CHECK(result == m1*m2);
    CHECK(result == i*result);
}

TEST(MatrixVectorMultiplication)
{
    Matrix<3, 3, float> m = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    Matrix<3, 3, float> i = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
    Vector<3, float> v = { 2, 4, 8 };
    Vector<3, float> result = { 34, 76, 118 };
    CHECK(result == (m*v));
    CHECK(v == (i*v));
}