#include "ray.h"
#include <cmath>
#include <iostream>

QVector4D Ray::boundingSphere(const float *vertices, int vcount) const
{
    float minx, maxx, miny, maxy, minz, maxz, x,y,z;

    if (vcount > 0)
    {
        minx = maxx = vertices[0];
        miny = maxy = vertices[1];
        minz = maxz = vertices[2];
    }
    else
    {
        return QVector4D(0,0,0,0);
    }
    for (int i = 1; i < vcount; i++)
    {
        x = vertices[i*3]; y = vertices[i*3+1]; z = vertices[i*3+2];
        if (x < minx) minx = x;
        if (x > maxx) maxx = x;
        if (y < miny) miny = y;
        if (y > maxy) maxy = y;
        if (z < minz) minz = z;
        if (z > maxz) maxz = z;
    }
    std::cout << "minmax" << std::endl;
    std::cout << "x: " << minx << " " << maxx << std::endl;
    std::cout << "y: " << miny << " " << maxy << std::endl;
    std::cout << "z: " << minz << " " << maxz << std::endl;
    x = (maxx + minx) / 2; // center components
    y = (maxy + miny) / 2;
    z = (maxz + minz) / 2;
    // distance between center and bounding box corner
    float radius = sqrt( pow(x - maxx,2) + pow(y - maxy,2) + pow(z - maxz,2) );
    return QVector4D(x,y,z,radius);
}

// return the index of the closest triangle
// that this ray intersects with. -1 if none
bool Ray::intersectsBoundingSphere(const float *vertices, int vcount, float &t1, float &t2) const
{
    // create the bounding sphere
    double x = 0.0, y = 0.0, z = 0.0,
          xc = 0.0, yc = 0.0, zc = 0.0;
    double k = 0.0;
//    for (int i = 0; i < vcount; i++)
//    {
//        xc += vertices[i*3];
//        yc += vertices[i*3+1];
//        zc += vertices[i*3+2];
//    }
//    xc /= vcount; yc /= vcount; zc /= vcount;
//    std::cout << xc << " " << yc << " " << zc << std::endl;
//    for (int i = 0; i < vcount; i++)
//    {
//        double x2 = pow(vertices[i*3] - xc,2); // (x - xc)^2
//        double y2 = pow(vertices[i*3+1] - yc,2); // (y - yc)^2
//        double z2 = pow(vertices[i*3+2] - zc,2); // (z - zc)^2
//        double dist = sqrt(x2 + y2 + z2); // (x - xc)^2 + (y - yc)^2 + (z - zc)^2 = d^2
//        if (dist > k) k = dist;
//    }
    QVector4D sphere = boundingSphere(vertices,vcount);
    xc = sphere.x(); yc = sphere.y(); zc = sphere.z(); k = sphere.w();
    std::cout << "center: " << xc << " " << yc << " " << zc << std::endl;

    std::cout << "k " << k << std::endl;
    std::cout << "r0 " << start.x() << ", " << start.y() << ", " << start.z() << std::endl;
    std::cout << "rhat " << direction.x() << ", " << direction.y() << ", " << direction.z() << std::endl;
    double xr = direction.x(), yr = direction.y(), zr = direction.z(),
           x0 = start.x(), y0 = start.y(), z0 = start.z();
    double A = pow(xr,2) + pow(yr,2) + pow(zr,2);
    double B = 2*((xr * (x0 - xc))
                + (yr * (y0 - yc))
                + (zr * (z0 - zc)));
    double C = pow((x0 - xc), 2) + pow((y0 - yc),2)
            + pow((z0 - zc), 2) - pow(k, 2);

    // t = (-B +-(B^2 - 4AC)^0.5) / 2A
    double sqB2_4AC = pow(B,2) - (4*A*C) ;
    std::cout << "A = " << A << std::endl;
    std::cout << "B = " << B << std::endl;
    std::cout << "C = " << C << std::endl;
    std::cout << "b^2 - 4ac = " << sqB2_4AC << std::endl;
    if (sqB2_4AC < 0.0)
        return false;
    else
    {
        sqB2_4AC = sqrt(sqB2_4AC);
        t1 = ((-B) + (sqB2_4AC)) / (2*A);
        t2 = ((-B) - (sqB2_4AC)) / (2*A);
        return true;
    }
}


bool Ray::intersects(MGL_Node *node)
{

}

bool Ray::intersects(MGL_SphereNode *node)
{
    return false;
}

float Ray::intersectsTriangles(float *vertices, int vcount)
{
    bool intersects = false;
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
        r_n = QVector3D::dotProduct(direction,normal);
        if (r_n != 0.0)
            t = (-D - QVector3D::dotProduct(start,normal)) / r_n;
        else
            t = -1.0; // if t < 0, intersection happens behind the starting point (eye)

        if (t > 0.0)
        {
            Pi = start + (t*direction);
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

                }
            }
        }
    }

    if (intersects)
        return distance;
    else
        return NAN;
}
