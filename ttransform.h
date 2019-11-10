#ifndef TTRANSFORM_H
#define TTRANSFORM_H

#include <QMatrix4x4>

class TTransform
{
public:
    TTransform();
    ~TTransform();
    
    QMatrix4x4 transformMatrix() const;
    void swapTransformOrder();
    void swapAxisOrder();

    void translate(float,int);
    void rotate(float,int);
    void scale(float,int);

private:
    QMatrix4x4 scaleValue, scaleX, scaleY, scaleZ; // needed for linear scale operations
    QMatrix4x4 translateX, translateY, translateZ;
    QMatrix4x4 rotateX, rotateY, rotateZ;
    
    int transformOrder = 0;
    int axisOrder = 0;
};

#endif // TTRANSFORM_H
