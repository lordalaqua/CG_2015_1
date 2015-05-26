#ifndef FrameBuffer_h__
#define FrameBuffer_h__
#include "AlmostGLParameters.h"
#include <vector>

namespace AlmostGL
{
    class FrameBuffer
    {
    public:
        FrameBuffer() {}
        FrameBuffer(int w, int h, float v = 0.f);
        void reset(int w, int h);
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
}
#endif // FrameBuffer_h__
