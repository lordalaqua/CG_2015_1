#ifndef Triangle3D_h__
#define Triangle3D_h__

#include "Vector3D.h"

class Triangle3D
{
public:
    Vector3f vertex[3];
    Vector3f normal[3];
    int material_index[3];
    Vector3f face_normal;
};

#endif // Triangle3D_h__