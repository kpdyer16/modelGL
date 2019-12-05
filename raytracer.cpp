#include "raytracer.h"
#include <iostream>

RayTracer::RayTracer()
{

}



inline void RayTracer::convert(int x, int y,
             float &cx, float &cy,
             const int w, const int h)
{
    // (0,w-1) (0,h-1) -> (-1,1) (-1,1)
    cx = (float(2*x) / w) - 1;
    cy = 1 - (float(2*y) / h);
}

void RayTracer::printConvertedPixels(int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float x,y;
            convert(j,i,x,y,width,height);
            std::cout << "x: " << x << "\ny: " << y << std::endl;
        }
    }
}


void RayTracer::traceRay()
{

}
