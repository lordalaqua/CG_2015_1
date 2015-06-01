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
#include "TextureParameters.h"

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
        void runVertexPipeline(const Model3D& model);
        void runRasterization();
        void bindTexture(std::string filename);
    public:
        Camera camera;
        Viewport viewport;
        WindingOrder winding_order;
        PolygonMode polygon_mode;
        LightParameters light;
        std::vector<Triangle> triangles;
        FrameBuffer buffer;
        TextureParameters texture;
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
        inline void writeToBuffer(Vertex v, FrameBuffer& buffer);
        Vector3f calculateTextureColor(float x, float y);
        float degreeToRadians(float angle);
        Vector3f bilinearInterpolatePixel(float x, float y, int level=0);
        template<typename T>
        T interpolate(T start, T end, float alpha);

        void startMipMapLine(int start, int end)
        {
            mipmap_line_start = true;
        }
        int mipmap_last_x;
        Vector2f mipmap_last_x_tex;
        int mipmap_last_y;
        Vector2f mipmap_last_y_tex;
        bool mipmap_line_start;
    };
} // namespace AlmostGL
#endif // AlmostGLPipeline_h__
