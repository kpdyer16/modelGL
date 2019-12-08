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
    void setAmbient(Color);

    void setDiffuse(float,float,float);
    void setDiffuse(Color);

    void setSpecular(float,float,float);
    void setSpecular(Color);

private:
    std::string name;
    Color ambient, diffuse, specular;
};

#endif // MATERIAL_H
