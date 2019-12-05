#ifndef TTRANSFORM_H
#define TTRANSFORM_H

#include <QMatrix4x4>

struct TTransform
{
    TTransform();
    ~TTransform();
    
    QMatrix4x4 transformMatrix() const;

    void setRotationAxisOrder(int order); // recalculates mat_rotate
    void setTransformOrder(int order);
    void calculateRotationMatrix();

    float scaleFactorX, scaleFactorY, scaleFactorZ;
    QMatrix4x4 mat_translate;
    QMatrix4x4 mat_rotate,
               mat_rotateX, mat_rotateY, mat_rotateZ;
    
    int transformOrder = 0;
    int axisOrder = 0;
};

#endif // TTRANSFORM_H
