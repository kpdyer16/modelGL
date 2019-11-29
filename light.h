#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <rgb.h>

class Light
{
public:
    Light() : Light(0.0,0.0,0.0) {}
    Light(float x, float y, float z);

    void setLocation(float x, float y, float z);

    void setAmbient(float,float,float);
    void setAmbient(RGB);

    void setDiffuse(float,float,float);
    void setDiffuse(RGB);

    void setSpecular(float,float,float);
    void setSpecular(RGB);

    void setFalloff(float);
    void setMaxDistance(float);

private:
    QVector3D location;
    RGB ambient, diffuse, specular;
    float falloff, max_dist;
};

#endif // LIGHT_H
