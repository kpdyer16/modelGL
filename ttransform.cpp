#include "ttransform.h"

TTransform::TTransform()
{
    scaleX = QMatrix4x4(0.5,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);
    scaleY = QMatrix4x4(0,0,0,0, 0,0.5,0,0, 0,0,0,0, 0,0,0,0);
    scaleZ = QMatrix4x4(0,0,0,0, 0,0,0,0, 0,0,0.5,0, 0,0,0,0);
    scaleValue.setToIdentity();
}

TTransform::~TTransform()
{

}

void TTransform::swapAxisOrder()
{
    if (axisOrder == 0)
        axisOrder = 1;
    else
        axisOrder = 0;
}

void TTransform::swapTransformOrder()
{
    if (transformOrder == 0)
        transformOrder = 1;
    else
        transformOrder = 0;
}

QMatrix4x4 TTransform::transformMatrix() const
{
    QMatrix4x4 rotation, xform;
    if (axisOrder == 0)
    {
        // XYZ
        rotation = rotateZ * rotateY * rotateX;
    }
    else
    {
        // YXZ
        rotation = rotateZ * rotateX * rotateY;
    }
    
    if (transformOrder == 0)
    {
        // SRT
        xform = (translateX * translateY * translateZ) * rotation * (scaleValue);
    }
    else
    {
        // STR
        xform = rotation * (translateX * translateY * translateZ) * (scaleValue);
    }

    return xform;
}

void TTransform::translate(float distance, int axis)
{
    switch(axis)
    {
    case 0: translateX.translate(distance,0,0);
        break;
    case 1: translateY.translate(0,distance,0);
        break;
    case 2: translateZ.translate(0,0,distance);
        break;
    }
}

void TTransform::rotate(float angle, int axis)
{
    switch(axis)
    {
    case 0: rotateX.rotate(angle,QVector3D(1,0,0));
        break;
    case 1: rotateY.rotate(angle,QVector3D(0,1,0));
        break;
    case 2: rotateZ.rotate(angle,QVector3D(0,0,1));
        break;
    }
}

void TTransform::scale(float amount,int axis)
{
    switch(axis)
    {
    case 0: if (amount > 0) scaleValue += scaleX;
            else if (amount < 0) scaleValue -= scaleX;
        break;
    case 1: if (amount > 0) scaleValue += scaleY;
            else if (amount < 0) scaleValue -= scaleY;
        break;
    case 2: if (amount > 0) scaleValue += scaleZ;
            else if (amount < 0) scaleValue -= scaleZ;
        break;
    }
}
