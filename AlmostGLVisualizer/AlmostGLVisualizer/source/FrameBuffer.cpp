#include "FrameBuffer.h"
namespace AlmostGL
{
    FrameBuffer::FrameBuffer(int w, int h, float v /*= 0.f*/) 
        : data(w*h * 3, v), 
        width(w), 
        height(h)
    {
        //empty
    }

    void FrameBuffer::reset(int w, int h)
    {
        data.clear();
        data.resize(w*h * 3);
        width = w; height = h;
    }
}