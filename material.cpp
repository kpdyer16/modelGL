#include "material.h"

void Material::setAmbient(float r, float g, float b)
{
    ambient = Color(r,g,b);
}
void Material::setAmbient(Color rgb)
{
    ambient = rgb;
}

void Material::setDiffuse(float r, float g, float b)
{
    diffuse = Color(r,g,b);
}
void Material::setDiffuse(Color rgb)
{
    diffuse = rgb;
}

void Material::setSpecular(float r, float g, float b)
{
    specular = Color(r,g,b);
}
void Material::setSpecular(Color rgb)
{
    specular = rgb;
}
