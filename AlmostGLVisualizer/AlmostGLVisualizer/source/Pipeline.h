#ifndef AlmostGLPipeline_h__
#define AlmostGLPipeline_h__

#include <cmath>
#include <algorithm>

#include "Vector.h"
#include "Matrix.h"
#include "Model3D.h"
#include "Camera.h"
#include "Material.h"
#include "Viewport.h"
#include "AlmostGLParameters.h"
#include "FrameBuffer.h"
#include "LightParameters.h"

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
    class Pipeline
    {
    public:
        struct Vertex
        {
            Vector4f pos;
            Vector3f color;
            float interpolation_factor;
        };

        struct Triangle
        {
            Vertex v[3];
            bool removed;
        };

    public:
        void runVertexPipeline(const Model3D& model);
        void runRasterization();
    public:
        Camera camera;
        Viewport viewport;
        WindingOrder winding_order;
        PolygonMode polygon_mode;
        LightParameters light;
        std::vector<Triangle> triangles;
        FrameBuffer buffer;
        bool FUN = false;
    private:        
        void convertTrianglesAndCalculateColor(const Model3D& model);
        void modelViewProjectionTransform();
        void clipping();
        void perspectiveDivision();
        void viewportTransform();
        void backFaceCulling();
        Vector3f calculateIllumination(const Vector3f& vertex,
            const Vector3f& normal, const Material& material);
        void rasterTriangle(Triangle& triangle, FrameBuffer& buffer,
            PolygonMode mode);
        void bresenham(const Vertex& start, const Vertex& end, FrameBuffer& buffer);
        Vertex createInterpolated(const Vertex& start, const Vertex& end, int x, int y);
        inline void writeToBuffer(const Vertex& v, FrameBuffer& buffer);
        float degreeToRadians(float angle);
    };
} // namespace AlmostGL
#endif // AlmostGLPipeline_h__
