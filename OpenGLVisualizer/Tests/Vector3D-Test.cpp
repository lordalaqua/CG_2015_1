#include <UnitTest++.h>
#include "Vector3D.h"

TEST(Vector3DConstructorsAssignmentAndEqualityTest)
{
    Vector3D<int> v1(1, 2, 3);
    Vector3D<int> v2;
    Vector3D<int> v3(v2);
    Vector3D<int> v4{ 1, 2 , 3};
    Vector3D<int> v5 = v1;

    CHECK(v1 == Vector3D<int>(1, 2, 3));
    CHECK(v2 == v3);
    CHECK(v4 == v1);
    CHECK(v1 == v5);
}

TEST(Vector3DAdding)
{
    Vector3f v1(2, 10,2);
    Vector3f v2(-2, 5,2);
    Vector3f vAdd(0, 15,4);

    CHECK(vAdd == v1 + v2);
}

TEST(Vector3DSubtracting)
{
    Vector3i v1(2, 10,2);
    Vector3i v2(-2, 5,2);
    Vector3i vSub1(4, 5,0); // v1 - v2
    Vector3i vSub2(-4, -5,0); // v2 - v1

    CHECK(vSub1 == v1 - v2);
    CHECK(vSub2 == v2 - v1);
    CHECK(vSub1 == -vSub2);
}
TEST(Vector3DMultiplication)
{
    Vector3f v1(2, 10, 20);
    Vector3f v2(2, 5, 2);
    Vector3f vMul(4, 50, 40);   // v1*v2
    Vector3f vMulS(20, 100, 200); // 10*v1

    CHECK(vMul == v1 * v2);
    CHECK(vMulS == 10 * v1);
    CHECK(vMulS == v1 * 10.0);
}

TEST(Vector3DDivision)
{
    Vector3d v1(2, 10, 9);
    Vector3d v2(2, 5, 3);
    Vector3d vDiv1(1, 2, 3); // v1 / v2
    Vector3d vDiv2(1, 0.5,3/9.0); // v2 / v1
    Vector3d vDivS(1, 5, 4.5); // v1 / 2 

    CHECK(vDiv1 == v1 / v2);
    CHECK(vDiv2 == v2 / v1);
    CHECK(vDivS == v1 / 2);
}

TEST(Vector3DLength)
{
    Vector3i v1(10, 0, 0);
    Vector3i v2(0, 10, 0);
    Vector3i v3(0, 0, 10);
    Vector3i v4(6, 8, 0);
    Vector3i v5(0, 3, 4);
    Vector3i v6(9, 0, 12);

    CHECK(v1.length() == 10);
    CHECK(v2.length() == 10);
    CHECK(v3.length() == 10);
    CHECK(v4.length() == 10);
    CHECK(v5.length() == 5);
    CHECK(v6.length() == 15);
}

TEST(Vector3DNormalize)
{
    Vector3i v1(10, 0, 0);
    Vector3i v2(0, 10, 0);
    Vector3d v3(6, 8, 0);
    Vector3d v4{ 6, 8, 0 };

    CHECK(v1.normalize().length() == 1);
    CHECK(v2.normalize() == Vector3i(0, 1,0));
    CHECK(v2.normalize() == v2.normalize());
    CHECK_CLOSE(.6, v3.normalize().x, 0.0001);
    CHECK_CLOSE(.8, v3.y, 0.0001);
    CHECK(getNormalized(v4) == v3.normalize());
    CHECK(v4 == Vector3d(6, 8, 0));
}

TEST(Vector3DDotProduct)
{
    Vector3i v1(10, 0, 0);
    Vector3i v2(0, 10, 0);
    Vector3d v3(6, 8, 4);
    Vector3d v4{ 3, 4, 2 };
    Vector3d v5{ 4, 5, 6 };
    Vector3d v6{ 1, 2, 3 };

    CHECK(dotProduct(v1,v2) == 0);
    CHECK_CLOSE(v3.length()*v4.length(), dotProduct(v3, v4), 0.0001);
    CHECK(dotProduct(v3.normalize(), v4.normalize()) == 1);
    CHECK(dotProduct(v5, v6) == (4*1+5*2+6*3));
}

TEST(Vector3DCrossProduct)
{
    Vector3i v1(1, 0, 0);
    Vector3i v2(0, 1, 0);
    Vector3i v3(0, 0, 1);
    Vector3d v4{ 4, -2, 1 };
    Vector3d v5{ 1, -1, 3 };
    Vector3d v6{ -5, -11, -2 };

    CHECK(crossProduct(v1, v2) == v3);
    CHECK(crossProduct(v2, v1) == -v3);
    CHECK(crossProduct(v2, v3) == v1);
    CHECK(crossProduct(v3, v2) == -v1);
    CHECK(crossProduct(v3, v1) == v2);
    CHECK(crossProduct(v1, v3) == -v2);
    CHECK(Vector3d(-5,-11,-2) == crossProduct(v4, v5));
}

TEST(VectorRotation)
{
    Vector3f v1{ 1, 0, 0 };
    Vector3f v2{ 0, 1, 0 };
    Vector3f v3{ 0, 0, 1 };
    std::cout << rotate(v1, v2, 180);
    std::cin.get();
    CHECK_CLOSE(-v1.x,rotate(v1, v2, 180).x,0.001);
}