#ifndef RAY_H
#define RAY_H

#include <QVector4D>
#include <QMatrix4x4>

class Ray
{
public:
    Ray();
    Ray(const QVector3D &r0, const QVector3D &d); // doesn't calculate the ray

    //
    bool intersectsBoundingSphere(const float *vertices, int vcount, float &t1, float &t2) const;
    QVector4D boundingSphere(const float *vertices, int vcount) const;
private:
    QVector3D start, direction;
};

#endif // RAY_H
