#ifndef AlmostGLParameters_h__
#define AlmostGLParameters_h__
#include "Vector.h"

namespace AlmostGL
{
    struct Vertex
    {
        Vector4f pos;
        Vector3f color;
        Vector2f texture;
        float interpolation_factor;
    };

    struct Triangle
    {
        Vertex v[3];
        bool removed;
    };

    enum WindingOrder { CW, CCW };

    enum Color { RED = 0, GREEN = 1, BLUE = 2 };

    enum PolygonMode { FILL, WIREFRAME, POINTS };

    enum LightingMode { NONE, FLAT, SMOOTH };
}
#endif // AlmostGLParameters_h__
