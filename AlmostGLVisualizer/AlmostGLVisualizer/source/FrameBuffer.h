#ifndef FrameBuffer_h__
#define FrameBuffer_h__
#include "AlmostGLParameters.h"
#include <vector>
#include "AlmostGLParameters.h"

namespace AlmostGL
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        FrameBuffer(int w, int h, float v = 0.f);
        void reset(int w, int h);
        void operator()(int x, int y, Color color, float value);
        void writeVertex(const Vertex& v);
        std::vector<float> data;
        std::vector<std::vector<float>> depth;
        bool depth_check;
        int width;
        int height;
    };
}
#endif // FrameBuffer_h__
