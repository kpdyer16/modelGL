#include "material.h"

void Material::setAmbient(float r, float g, float b)
{
    ambient = RGB(r,g,b);
}
void Material::setAmbient(RGB rgb)
{
    ambient = rgb;
}

void Material::setDiffuse(float r, float g, float b)
{
    diffuse = RGB(r,g,b);
}
void Material::setDiffuse(RGB rgb)
{
    diffuse = rgb;
}

void Material::setSpecular(float r, float g, float b)
{
    specular = RGB(r,g,b);
}
void Material::setSpecular(RGB rgb)
{
    specular = rgb;
}
