#ifndef TextureParameters_h__
#define TextureParameters_h__

#include <string>
#include <vector>
#include "QImage"
#include "AlmostGLParameters.h"
#include "Vector.h"

/*
	Structure to hold texture mapping information for the AlmostGL pipeline
*/

namespace AlmostGL
{
    enum TextureFilter
    {
        NEAREST,
        LINEAR,
        MIPMAP
    };

    enum TextureMode
    {
        DECAL,
        MODULATE
    };

    struct TextureParameters
    {
        TextureParameters() : enabled(false), filter(LINEAR), mode(MODULATE) {}
        float getPixelColor(int x, int y, Color color, int level = 0);
        Vector3f getPixelColor(int x, int y, int level = 0);
        void loadImage(std::string filename);
        int width() { return mipmap_pyramid[0].width(); }
        int height() { return mipmap_pyramid[0].height(); }
    public:
        bool enabled;
        TextureFilter filter;
        TextureMode mode;
    private:
        std::vector<QImage> mipmap_pyramid;
        const int MAX_MIPMAP_LEVEL = 7;
    };
} // namespace
#endif // TextureParameters_h__
