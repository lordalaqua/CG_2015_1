#include <UnitTest++.h>
#include "Model3D.h"

TEST(Model3DReadTest)
{
    Model3D model("C:/Projects/CG_2015_1/AlmostGLVisualizer/AlmostGLVisualizer/Resources/cube.txt");
    
    //Check if material is read correctly
    CHECK(model.material(0).ambient == Vector3f(0.694f, 0.58f, 0.459f));
    CHECK(model.material(0).diffuse == Vector3f(0.992f, 0.941f, 0.863f));
    CHECK(model.material(0).specular == Vector3f(1.0, 1.0, 1.0));
    CHECK(model.material(0).shine == 0.25);

    // Check first triangle is read correctly
    CHECK(model.triangle(0).vertex[0] == Vector3f(-1.0, -1.0, -2.0));
    CHECK(model.triangle(0).vertex[1] == Vector3f(1.0, -1.0, -2.0));
    CHECK(model.triangle(0).vertex[2] == Vector3f(1.0, 1.0, -2.0));
    CHECK(model.triangle(0).normal[0] == Vector3f(0, 0, 1.0));
    CHECK(model.triangle(0).normal[1] == Vector3f(0, 0, 1.0));
    CHECK(model.triangle(0).normal[2] == Vector3f(0, 0, 1.0));
    CHECK(model.triangle(0).material_index[0] == 0);
    CHECK(model.triangle(0).material_index[1] == 0);
    CHECK(model.triangle(0).material_index[2] == 0);
    CHECK(model.triangle(0).face_normal == Vector3f(0, 0, 1.0));

    // Check if LAST triangle is read correctly
    CHECK(model.triangle(11).vertex[0] == Vector3f(-1.0, -1.0, -2.0));
    CHECK(model.triangle(11).vertex[1] == Vector3f(-1.0, -1.0, -4.0));
    CHECK(model.triangle(11).vertex[2] == Vector3f(1.0, -1.0, -4.0));
    CHECK(model.triangle(11).normal[0] == Vector3f(0, -1.0, 0));
    CHECK(model.triangle(11).normal[1] == Vector3f(0, -1.0, 0));
    CHECK(model.triangle(11).normal[2] == Vector3f(0, -1.0, 0));
    CHECK(model.triangle(11).material_index[0] == 0);
    CHECK(model.triangle(11).material_index[1] == 0);
    CHECK(model.triangle(11).material_index[2] == 0);
    CHECK(model.triangle(11).face_normal == Vector3f(0, -1.0, 0));

    CHECK(model.min == Vector3f(-1.0, -1.0, -4.0));
    CHECK(model.max == Vector3f(1.0, 1.0, -2.0));
}