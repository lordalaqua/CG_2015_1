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

/*
    AlmostGL namespace

    Functions described in this file compose the operations necessary to render
    3D scenes in an OpenGL-like fashion, transforming a description of 3D vertices 
    into a 2D pixel image, considering a virtual camera in the 3D space.

    The main function runAlmostGLPipeline performs the transformations on a 
    3D model received as a parameter, and returns the transformed vertices
    in the 2D pixel space.
*/
class AlmostGL
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

    enum WindingOrder { CW, CCW };

    enum Color { RED = 0, GREEN = 1, BLUE = 2 };

    enum PolygonMode { FILL, WIREFRAME, POINTS };

    enum LightingMode { NONE, FLAT, SMOOTH };

    struct LightSource
    {
        Vector3f position;
        Vector3f color;
    };

    struct LightParameters
    {
        LightParameters()
        : mode(NONE),
        ambient_color({ 1.f, 1.f, 1.f }),
        constant_attenuation(1.f),
        linear_attenuation(0.f),
        quadratic_attenuation(0.f) {}

        LightingMode mode;
        Vector3f ambient_color;
        float constant_attenuation;
        float linear_attenuation;
        float quadratic_attenuation;
        std::vector<LightSource> sources;
    };

    class FrameBuffer
    {
    public:
        FrameBuffer() {}
        FrameBuffer(int w, int h, float v = 0.f) : data(w*h * 3, v), width(w), height(h) {}
        void resize(int w, int h) { data.clear(); data.resize(w*h * 3); width = w; height = h; }
        float& operator()(int x, int y, Color color)
        {
            if (0 <= x && x < width && 0 <= y && y < height)
                return data[(y*width + x) * 3 + (int)color];
            else
                return data[0];
        }
        std::vector<float> data;
        int width;
        int height;
    };

    /* Main Pipeline function
     Transforms vertices in the 3D space into the appropriate 2D pixels in
     the view frustum and viewport provided.
    - Model contains vertices (triangles) to be drawn.
    - Camera contains camera position and orientation for frustum calculation.
    - VP(viewport) top,bottom,left,right parameters define viewport.
    */
public:
    void runVertexPipeline(const Model3D& model);
    void runRasterization();
public:
    Camera camera;
    Viewport vp;
    WindingOrder order;
    Material material;
    LightParameters light;
    std::vector<Triangle> triangles;
    FrameBuffer buffer;
    PolygonMode mode;
private:
    Vector3f calculateIllumination(const Vector3f& vertex, 
        const Vector3f& normal, const Material& material, const Camera& camera, 
        const LightParameters& light);
    void rasterTriangle(Triangle& triangle, FrameBuffer& buffer, 
        PolygonMode mode);
    void bresenham(const Vertex& start, const Vertex& end, FrameBuffer& buffer);
    Vertex createInterpolated(const Vertex& start,const Vertex& end,int x,int y);
    inline void writeToBuffer(const Vertex& v, FrameBuffer& buffer);
    float degreeToRadians(float angle);
};

#endif // AlmostGLPipeline_h__
