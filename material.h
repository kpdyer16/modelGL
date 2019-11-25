#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <rgb.h>

class Material
{
public:
    Material() : Material(1.0,1.0,1.0) {}
    Material(float r, float g, float b) :
        ambient(r,g,b), diffuse(r,g,b), specular(r,g,b) {}

    void setAmbient(float,float,float);
    void setAmbient(RGB);

    void setDiffuse(float,float,float);
    void setDiffuse(RGB);

    void setSpecular(float,float,float);
    void setSpecular(RGB);

private:
    std::string name;
    RGB ambient, diffuse, specular;
};

#endif // MATERIAL_H
