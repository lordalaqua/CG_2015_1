#ifndef LightParameters_h__
#define LightParameters_h__
#include <vector>
#include "Vector.h"
#include "AlmostGLParameters.h"

/*
	Structure for holding lighting information for the AlmostGL pipeline
*/

namespace AlmostGL
{
    struct LightSource
    {
        Vector3f position;
        Vector3f color;
    };

    struct LightParameters
    {
        LightParameters()
        : mode(NONE),
        ambient_color({ .5f, .5f, .5f }),
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
}
#endif // LightParameters_h__
