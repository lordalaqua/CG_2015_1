#ifndef Viewport_h__
#define Viewport_h__

/*
	Viewport class to hold information related to the viewport in the AlmostGL pipeline.
*/

class Viewport
{
public:
    Viewport(float t, float r, float b, float l)
    : top(t), bottom(b), left(l), right(r) {}
    Viewport() : Viewport(0, 0, 0, 0) {}
public:
    float top;
    float bottom;
    float left;
    float right;
};

#endif // Viewport_h__