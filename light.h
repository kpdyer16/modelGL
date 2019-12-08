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
    void setAmbient(Color);

    void setDiffuse(float,float,float);
    void setDiffuse(Color);

    void setSpecular(float,float,float);
    void setSpecular(Color);

    void setFalloff(float);
    void setMaxDistance(float);

    QVector3D getLocation() { return location; }
private:
    QVector3D location;
    Color ambient, diffuse, specular;
    float falloff, max_dist;
};

#endif // LIGHT_H
