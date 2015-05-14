#ifndef AlmostGLPipeline_h__
#define AlmostGLPipeline_h__
#include <cmath>
#include <algorithm>
#include "Vector.h"
#include "Matrix.h"
#include "Model3D.h"
#include "Camera.h"

/*
    AlmostGL namespace

    Functions described in this file compose the operations necessary to render
    3D scenes in an OpenGL-like fashion, transforming a description of 3D vertices 
    into a 2D pixel image, considering a virtual camera in the 3D space.

    The main function runAlmostGLPipeline performs the transformations on a 
    3D model received as a parameter, and returns the transformed vertices
    in the 2D pixel space.
*/
namespace AlmostGL
{
    /*
        4-Dimensional triangle for calculations with homogeneous coordinates
    */
    struct Triangle4D
    {
        Vector<4, float> vertex[3];
        bool clipped;
    };

    struct ViewPort
    {
        ViewPort(float t, float r, float b, float l)
            : top(t), bottom(b), left(l), right(r) {}
        ViewPort() : ViewPort(0, 0, 0, 0) {}
        float top;
        float bottom;
        float left;
        float right;
    };

    enum WindingOrder
    {
        CW,
        CCW
    };

    /* Main Pipeline function
     Transforms vertices in the 3D space into the appropriate 2D pixels in
     the view frustum and viewport provided.
    - Model contains vertices (triangles) to be drawn.
    - Camera contains camera position and orientation for frustum calculation.
    - VP(viewport) top,bottom,left,right parameters define viewport.
    */
    std::vector<Triangle4D> runPipeline(const Model3D& model, const Camera& camera,
        ViewPort vp, WindingOrder order);

    float degreeToRadians(float angle);
}

#endif // AlmostGLPipeline_h__
