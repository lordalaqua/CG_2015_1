#include <UnitTest++.h>
#include "Vector.h"
#include <iostream>

TEST(VectorConstructorsAssignmentAndEqualityTest)
{
    Vector<3, int> v1 = { 1, 2, 3 };
    Vector<3,int> v2;
    Vector<3,int> v3(v2);
    Vector<3,int> v4{ 1, 2, 3 };
    Vector<3,int> v5 = v1;

    CHECK(v5[0]-- == v5.x);
    CHECK(v5[1]-- == v5.y);
    CHECK(v5.z-- == v5[2]);
    CHECK(v5.w == v5[0]);
    CHECK(v1 == v1);
    CHECK(v2 == v3);
    CHECK(v4 == v1);
    CHECK(v1 != v5);
}

TEST(VectorAdding)
{
    Vector<3, int> v1{ 2, 10, 2 };
    Vector<3, int> v2{ -2, 5, 2 };
    Vector<3, int> vAdd{0, 15, 4};

    CHECK(vAdd == v1 + v2);
}

TEST(VectorSubtracting)
{
    Vector<3, int> v1{ 2, 10, 2 };
    Vector<3, int> v2{ -2, 5, 2 };
    Vector<3, int> vSub1{ 4, 5, 0 }; // v1 - v2
    Vector<3, int> vSub2{ -4, -5, 0 }; // v2 - v1

    CHECK(vSub1 == v1 - v2);
    CHECK(vSub2 == v2 - v1);
    CHECK(vSub1 == -vSub2);
}
TEST(VectorMultiplication)
{
    Vector<3, int> v1{ 2, 10, 20 };
    Vector<3, int> v2{ 2, 5, 2 };
    Vector<3, int> vMul{ 4, 50, 40 };   // v1*v2
    Vector<3, int> vMulS{ 20, 100, 200 }; // 10*v1

    CHECK(vMul == v1 * v2);
    CHECK(vMulS == 10 * v1);
    CHECK(vMulS == v1 * 10.0);
}

TEST(VectorDivision)
{
    Vector<3,double> v1{2, 10, 9};
    Vector<3,double> v2{2, 5, 3};
    Vector<3,double> vDiv1{1, 2, 3}; // v1 / v2
    Vector<3,double> vDiv2{1, 0.5, 3 / 9.0}; // v2 / v1
    Vector<3,double> vDivS{1, 5, 4.5}; // v1 / 2 

    CHECK(vDiv1 == v1 / v2);
    CHECK(vDiv2 == v2 / v1);
    CHECK(vDivS == v1 / 2);
}

TEST(VectorLength)
{
    Vector<3,int> v1{10, 0, 0};
    Vector<3,int> v2{0, 10, 0};
    Vector<3,int> v3{0, 0, 10};
    Vector<3,int> v4{6, 8, 0};
    Vector<3,int> v5{0, 3, 4};
    Vector<3,int> v6{9, 0, 12};

    CHECK(v1.length() == 10);
    CHECK(v2.length() == 10);
    CHECK(v3.length() == 10);
    CHECK(v4.length() == 10);
    CHECK(v5.length() == 5);
    CHECK(v6.length() == 15);
}

TEST(VectorNormalize)
{
    Vector<3,float> v1{10, 0, 0};
    Vector<3,float> v2{0, 10, 0};
    Vector<3,float> v3{6, 8, 0};
    Vector<3,float> v4{ 6, 8, 0 };
    Vector<3, float> unit{ 0, 1, 0 };
    Vector<3, float> square{ 6, 8, 0 };

    CHECK(v1.normalize().length() == 1);
    CHECK(v2.normalize() == unit);
    CHECK(v2.normalize() == v2.normalize());
    CHECK_CLOSE(.6, v3.normalize()[0], 0.0001);
    CHECK_CLOSE(.8, v3[1], 0.0001);
    CHECK(getNormalized(v4) == v3.normalize());
    CHECK(v4 == square);
}

TEST(VectorDotProduct)
{
    Vector<3,float> v1{10, 0, 0};
    Vector<3,float> v2{0, 10, 0};
    Vector<3,float> v3{6, 8, 4};
    Vector<3,float> v4{ 3, 4, 2 };
    Vector<3,float> v5{ 4, 5, 6 };
    Vector<3,float> v6{ 1, 2, 3 };

    CHECK(dotProduct(v1, v2) == 0);
    CHECK_CLOSE(v3.length()*v4.length(), dotProduct(v3, v4), 0.0001);
    CHECK(dotProduct(v3.normalize(), v4.normalize()) == 1);
    CHECK(dotProduct(v5, v6) == (4 * 1 + 5 * 2 + 6 * 3));
}

TEST(VectorCrossProduct)
{
    Vector<3,int> v1{1, 0, 0};
    Vector<3,int> v2{0, 1, 0};
    Vector<3,int> v3{0, 0, 1};
    Vector<3,int> v4{ 4, -2, 1 };
    Vector<3,int> v5{ 1, -1, 3 };
    Vector<3,int> v6{ -5, -11, -2 };

    CHECK(crossProduct(v1, v2) == v3);
    CHECK(crossProduct(v2, v1) == -v3);
    CHECK(crossProduct(v2, v3) == v1);
    CHECK(crossProduct(v3, v2) == -v1);
    CHECK(crossProduct(v3, v1) == v2);
    CHECK(crossProduct(v1, v3) == -v2);
    CHECK(v6 == crossProduct(v4, v5));
}

TEST(VectorRotation)
{
    Vector<3,int> v1{ 1, 0, 0 };
    Vector<3,int> v2{ 0, 1, 0 };
    Vector<3,int> v3{ 0, 0, 1 };
    CHECK_CLOSE(-v1[0], rotate(v1, v2, 180)[0], 0.001);
}