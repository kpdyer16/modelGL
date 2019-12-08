#ifndef RAY_H
#define RAY_H

#include <QVector4D>
#include <QMatrix4x4>
#include <mgl_node.h>
#include <mgl_spherenode.h>

class Ray
{
public:
    Ray() : Ray(QVector3D(0,0,0),QVector3D(0,0,1)) {}
    Ray(const QVector3D &r0, const QVector3D &d) : start(r0), direction(d) {} // doesn't calculate the ray

    //
    bool intersectsBoundingSphere(const float *vertices, int vcount, float &t1, float &t2) const;
    QVector4D boundingSphere(const float *vertices, int vcount) const;

    bool intersects(MGL_Node *);
    bool intersects(MGL_SphereNode *);
    float intersectsTriangles(float *data, int vcount);
    const QVector3D start;
    const QVector3D direction;
};

#endif // RAY_H
