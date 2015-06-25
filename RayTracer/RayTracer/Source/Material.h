#pragma once

#include "Vector.h"
/*
    3D model material information
    Diffuse, ambient, specular colors, and material shine.
*/
class Material
{
public:
    
    Material(Vector3f ambient_color, Vector3f diffuse_color, Vector3f specular_color, float material_shine);
    Material() : Material({ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 0) {}

public:
    Vector3f ambient;
    Vector3f diffuse;
    Vector3f specular;
    float shine;
};