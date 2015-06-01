#include "TextureParameters.h"




float AlmostGL::TextureParameters::getPixelColor(int x, int y, Color color, int level/*=0*/)
{
    QRgb pixel = mipmap_pyramid[level].pixel(x, y);
    switch (color)
    {
    case RED:
        return qRed(pixel) / 255.f;
        break;
    case GREEN:
        return qBlue(pixel) / 255.f; // QImage is RBG??? not sure, but working
        break;
    case BLUE:
        return qGreen(pixel) / 255.f;
        break;
    }
}

Vector3f AlmostGL::TextureParameters::getPixelColor(int x, int y, int level/* = 0*/)
{
    Vector3f color;
    color[0] = getPixelColor(x, y, RED);
    color[1] = getPixelColor(x, y, BLUE);
    color[2] = getPixelColor(x, y, GREEN);
    return color;
}

void AlmostGL::TextureParameters::loadImage(std::string filename)
{
    QImage texture(filename.c_str());
    texture = texture.mirrored();
    mipmap_pyramid.push_back(texture);
    int w = texture.width(), h = texture.height();
    for (int i = 1; i <= MAX_MIPMAP_LEVEL; ++i)
    {
        mipmap_pyramid.push_back(texture.scaled(QSize(w/(i * 2), h/(i * 2))));
    }
}
