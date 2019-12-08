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


Color RayTracer::traceRay(const Ray &r, int depth, bool specularOnly, MGL_Node *world)
{
    bool objIntersect = false;
    float tIntersect;
    for (auto obj = world->begin(); obj != world->end(); obj++)
    {
        int vcount = (*obj)->getVertexCountRecursive();
        float *vertices = new float[vcount*3];
        (*obj)->fillRaw(vertices);

        // **************

        float distance = 0.0, D, t, r_n; // r_n is the dot product of rhat and normal

        QVector3D triangle[3], normal, Pi, A,B, Sign;
        for (int i = 0; i < vcount / 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                // triangle i.x0 = vertices[i*9]
                // triangle i.x1 = vertices[i*9+3]
                // so, triangle i.xj = vertices[i*9+j*3]
                triangle[j] = QVector3D(vertices[i*9+j*3],vertices[i*9+j*3+1],vertices[i*9+j*3+2]);
            }
            normal = QVector3D::crossProduct(triangle[1]-triangle[0],triangle[2]-triangle[0]).normalized();
            D = QVector3D::dotProduct(-triangle[0],normal);
            r_n = QVector3D::dotProduct(r.direction,normal);
            if (r_n != 0.0)
                t = (-D - QVector3D::dotProduct(r.start,normal)) / r_n;
            else
                t = -1.0; // if t < 0, intersection happens behind the starting point (eye)

            if (t > 0.0)
            {
                Pi = r.start + (t*r.direction);
                A = Pi - triangle[2];
                B = triangle[0] - triangle[2];
                Sign = QVector3D::crossProduct(A,B).normalized();
                // Requirement
                // Pi-v2 x v0-v2 = Pi-v0 x v1-v0 = Pi-v1 x v2-v1
                A = Pi - triangle[0]; B = triangle[1] - triangle[0];
                if (qFuzzyCompare(Sign,QVector3D::crossProduct(A,B).normalized()))
                {
                    A = Pi - triangle[1]; B = triangle[2] - triangle[1];
                    if (qFuzzyCompare(Sign,QVector3D::crossProduct(A,B).normalized()))
                    {
                        // it does intersect
                        // now we want to see if light reflects on this spot
                        for (auto &light: lights)
                        {

                        }

                    }
                }
            }
        }
        // **********
        delete[] vertices;
    }
}
