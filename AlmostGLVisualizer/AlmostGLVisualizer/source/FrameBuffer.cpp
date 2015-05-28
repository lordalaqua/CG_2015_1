#include "FrameBuffer.h"
#include <limits>

namespace AlmostGL
{
    FrameBuffer::FrameBuffer(int w, int h, float v /*= 0.f*/) 
        : data(w*h * 3, v), 
        width(w), 
        height(h)
    {
        depth_check = true;
    }

    FrameBuffer::FrameBuffer()
    {
        depth_check = true;
    }

    void FrameBuffer::reset(int w, int h)
    {
        data.clear();
        data.resize(w*h * 3);
        depth.clear();
        depth.resize(w);
        for (auto& row : depth)
            row.resize(h, std::numeric_limits<float>::max());
        width = w; height = h;
    }

    void FrameBuffer::operator()(int x, int y, Color color, float value)
    {
        if (0 <= x && x < width && 0 <= y && y < height)
        {
            data[(y*width + x) * 3 + (int)color] = value;     
        }
            
    }

    void FrameBuffer::writeVertex(const Vertex& v)
    {
        int x = v.pos.x;
        int y = v.pos.y;
        if (0 <= x && x < width && 0 <= y && y < height)
        {
            if (depth_check)
            {
                if (v.pos.z < depth[x][y])
                {
                    depth[x][y] = v.pos.z;
                    data[(y*width + x) * 3 + (int)AlmostGL::RED] = v.color[0];
                    data[(y*width + x) * 3 + (int)AlmostGL::GREEN] = v.color[1];
                    data[(y*width + x) * 3 + (int)AlmostGL::BLUE] = v.color[2];
                }
            }
            else
            {
                data[(y*width + x) * 3 + (int)AlmostGL::RED] = v.color[0];
                data[(y*width + x) * 3 + (int)AlmostGL::GREEN] = v.color[1];
                data[(y*width + x) * 3 + (int)AlmostGL::BLUE] = v.color[2];
            }            
        }
    }

}