#ifndef Triangle3D_h__
#define Triangle3D_h__

#include "Vector.h"
/*
    3D triangle (semi-plane) wrapper
    Stores information about a triangle in 3D space for rendering,
    such as vertices position, normal vector and an index
    for the vertex material.
*/

class Triangle3D
{
public:
    Vector3f vertex[3];
    Vector3f normal[3];
    Vector2f texture[3];
    int material_index[3];
    Vector3f face_normal;
};

#endif // Triangle3D_h__