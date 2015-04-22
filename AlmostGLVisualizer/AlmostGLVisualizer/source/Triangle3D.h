#ifndef Triangle3D_h__
#define Triangle3D_h__

#include "Vector3D.h"
/*
    3D triangle (semi-plane) wrapper
    Stores information about a CG triangle for rendering,
    such as vertices position, normal vector and an index
    for the vertex material. Used by Model3D class.
*/

class Triangle3D
{
public:
    Vector3f vertex[3];
    Vector3f normal[3];
    int material_index[3];
    Vector3f face_normal;
};

#endif // Triangle3D_h__