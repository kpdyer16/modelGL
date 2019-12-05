#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QVector4D>

struct Ray
{
    QVector3D r0, rHat;
};

class RayTracer
{
public:
    RayTracer();

    void printConvertedPixels(int width, int height);
    inline void convert(int x, int y,
                 float &cx, float &cy,
                 const int w, const int h);

    void traceRay();

private:


};

#endif // RAYTRACER_H
