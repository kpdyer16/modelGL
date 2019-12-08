#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QVector4D>
#include <rgb.h>
#include <mgl_node.h>
#include <ray.h>
#include <light.h>
#include <material.h>
#include <list>
#include <map>

class RayTracer
{
public:
    RayTracer();

    void printConvertedPixels(int width, int height);
    inline void convert(int x, int y,
                 float &cx, float &cy,
                 const int w, const int h);

    Color traceRay(const Ray &r, int depth, bool specularOnly, MGL_Node *world);


private:
    std::list<Light> lights;
    std::map<std::string,Material> materials;
};

#endif // RAYTRACER_H
