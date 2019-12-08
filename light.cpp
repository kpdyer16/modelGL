#include "light.h"

Light::Light(float x, float y, float z) : location(x,y,z), ambient(), diffuse(),
                                          specular(), falloff(0.0), max_dist(-1.0)
{

}

void Light::setLocation(float x, float y, float z)
{
    location = QVector3D(x,y,z);
}

void Light::setAmbient(float r, float g, float b)
{
    ambient = Color(r,g,b);
}
void Light::setAmbient(Color rgb)
{
    ambient = rgb;
}

void Light::setDiffuse(float r, float g, float b)
{
    diffuse = Color(r,g,b);
}
void Light::setDiffuse(Color rgb)
{
    diffuse = rgb;
}

void Light::setSpecular(float r, float g, float b)
{
    specular = Color(r,g,b);
}
void Light::setSpecular(Color rgb)
{
    specular = rgb;
}

void Light::setFalloff(float fo)
{
    falloff = fo;
}
void Light::setMaxDistance(float md)
{
    max_dist = md;
}

