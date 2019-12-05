#include "ttransform.h"

TTransform::TTransform()
{
    scaleFactorX = 1.0;
    scaleFactorY = 1.0;
    scaleFactorZ = 1.0;
}

TTransform::~TTransform()
{

}



QMatrix4x4 TTransform::transformMatrix() const
{
    // 6 possible permutations
    // SRT
    // STR
    // TSR
    // RST
    // RTS
    // TRS
    QMatrix4x4 scale; scale.scale(scaleFactorX, scaleFactorY, scaleFactorZ);
    switch(transformOrder)
    {
    case 0: return mat_translate * mat_rotate * scale;
    case 1: return mat_rotate * mat_translate * scale;
    case 2: return mat_rotate * scale * mat_translate;
    case 3: return mat_translate * scale * mat_rotate;
    case 4: return scale * mat_translate * mat_rotate;
    case 5: return scale * mat_rotate * mat_translate;
    default: return QMatrix4x4();
    }
}

void TTransform::setTransformOrder(int order)
{
    if (order < 6 && order >= 0)
        transformOrder = order;
    else
        order = 0;
}
void TTransform::setRotationAxisOrder(int order)
{
    axisOrder = order < 6 && order >= 0 ? order : 0;
    calculateRotationMatrix();
}

void TTransform::calculateRotationMatrix()
{
    switch(axisOrder)
    {
    case 0: mat_rotate = mat_rotateZ * mat_rotateY * mat_rotateX;
        break;
    case 1: mat_rotate = mat_rotateY * mat_rotateZ * mat_rotateX;
        break;
    case 2: mat_rotate = mat_rotateY * mat_rotateX * mat_rotateZ;
        break;
    case 3: mat_rotate = mat_rotateZ * mat_rotateX * mat_rotateY;
        break;
    case 4: mat_rotate = mat_rotateX * mat_rotateZ * mat_rotateY;
        break;
    case 5: mat_rotate = mat_rotateX * mat_rotateY * mat_rotateZ;
        break;
    }
}
