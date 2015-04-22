#include <UnitTest++.h>
#include "Model3D.h"

TEST(Model3DReadTest)
{
    Model3D model("C:/Projects/CG_2015_1/AlmostGLVisualizer/AlmostGLVisualizer/Resources/cube.txt");
    
    //Check if material is read correctly
    Vector3f ambient = { 0.694f, 0.58f, 0.459f };
    Vector3f diffuse = { 0.992f, 0.941f, 0.863f };
    Vector3f specular = { 1.0, 1.0, 1.0 };
    CHECK(model.material(0).ambient == ambient);
    CHECK(model.material(0).diffuse == diffuse);
    CHECK(model.material(0).specular == specular);
    CHECK(model.material(0).shine == 0.25);

    // Check first triangle is read correctly
    Vector3f vfirst0 = { -1.0, -1.0, -2.0 }, vfirst1 = { 1.0, -1.0, -2.0 },
        vfirst2 = { 1.0, 1.0, -2.0 };
    Vector3f nfirst = { 0, 0, 1.0 };
    Vector3f face_first = { 0, 0, 1.0 };

    CHECK(model.triangle(0).vertex[0] == vfirst0);
    CHECK(model.triangle(0).vertex[1] == vfirst1);
    CHECK(model.triangle(0).vertex[2] == vfirst2);
    CHECK(model.triangle(0).normal[0] == nfirst);
    CHECK(model.triangle(0).normal[1] == nfirst);
    CHECK(model.triangle(0).normal[2] == nfirst);
    CHECK(model.triangle(0).material_index[0] == 0);
    CHECK(model.triangle(0).material_index[1] == 0);
    CHECK(model.triangle(0).material_index[2] == 0);
    CHECK(model.triangle(0).face_normal == face_first);

    Vector3f vlast0 = { -1.0, -1.0, -2.0 }, vlast1 = { -1.0, -1.0, -4.0 },
        vlast2 = { 1.0, -1.0, -4.0 };
    Vector3f nlast = { 0, -1.0, 0 };
    Vector3f face_last = { 0, -1.0, 0 };

    // Check if LAST triangle is read correctly
    CHECK(model.triangle(11).vertex[0] == vlast0);
    CHECK(model.triangle(11).vertex[1] == vlast1);
    CHECK(model.triangle(11).vertex[2] == vlast2);
    CHECK(model.triangle(11).normal[0] == nlast);
    CHECK(model.triangle(11).normal[1] == nlast);
    CHECK(model.triangle(11).normal[2] == nlast);
    CHECK(model.triangle(11).material_index[0] == 0);
    CHECK(model.triangle(11).material_index[1] == 0);
    CHECK(model.triangle(11).material_index[2] == 0);
    CHECK(model.triangle(11).face_normal == face_last);

    Vector3f min = { -1.0, -1.0, -4.0 }, max = { 1.0, 1.0, -2.0 };
    CHECK(model.min == min);
    CHECK(model.max == max);
}