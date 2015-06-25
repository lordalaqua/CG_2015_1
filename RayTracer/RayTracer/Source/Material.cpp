#include "Material.h"

Material::Material(Vector3f ambient_color, Vector3f diffuse_color, Vector3f specular_color, float material_shine) 
: ambient(ambient_color)
, diffuse(diffuse_color)
, specular(specular_color)
, shine(material_shine)
{

}
