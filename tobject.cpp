#include "tobject.h"
#include <iostream>

const QVector4D TObject::white = QVector4D(1.0,1.0,1.0,1.0);
const QVector4D TObject::red = QVector4D(1.0,0.0,0.0,1.0);
const QVector4D TObject::blue = QVector4D(0.0,0.0,0.25,1.0);

TObject::TObject()
{
    parent = nullptr;
}

TObject::~TObject()
{
    deleteChildren();
}

std::list<TObject *>::iterator TObject::begin()
{
    return children.begin();
}

std::list<TObject *>::iterator TObject::end()
{
    return children.end();
}

void TObject::swapTransformOrder()
{
    transform.swapTransformOrder();
}

void TObject::swapAxisOrder()
{
    transform.swapAxisOrder();
}


void TObject::addChild(TObject *child)
{
    if (child->parent != nullptr)
    {
        std::cout << "Cannot add this node, already has a parent.\n"
                  << "Must call removeChild() on its parent first" << std::endl;
        return;
    }
    children.push_back(child);
    child->parent = this;
}

void TObject::removeChild(std::list<TObject *>::iterator &childIter)
{
    TObject *child = *childIter;
    child->parent = nullptr;
    children.erase(childIter);
}

void TObject::deleteChildren()
{
     for (auto &child: children)
     {
         delete child;
     }
     children.clear();
}


void TObject::addTriangle(QVector4D triangle[3])
{
    if (vertexCount < 300)
    {
        for (int i = 0; i < 3; i++)
        {
            vertices[vertexCount][0] = triangle[i].x();
            vertices[vertexCount][1] = triangle[i].y();
            vertices[vertexCount][2] = triangle[i].z();
            vertices[vertexCount++][3] = triangle[i].w();
        }
    }
    else
    {
        std::cout << "Achieved maximum number of triangles for this object" << std::endl;
    }
}

void TObject::translate(float distance, int axis)
{
    transform.translate(distance,axis);
}

void TObject::rotate(float angle, int axis)
{
    transform.rotate(angle,axis);
}

void TObject::scale(float amount, int axis)
{
    transform.scale(amount,axis);
}

void TObject::render(QOpenGLShaderProgram *pShaderProgram,
                     const QMatrix4x4 &perspective,
                     const QMatrix4x4 &camera,
                     const QMatrix4x4 &transform,
                     int color) const
{

    QVector4D renderColor;
    switch(color)
    {
    case 0: renderColor = white; break;
    case 1: renderColor = red; break;
    case 2: renderColor = blue; break;
    default: renderColor = white;
    }

    // perform matrix operations
    QMatrix4x4 t = transform * this->transform.transformMatrix();

    QOpenGLBuffer *pBuffer = new QOpenGLBuffer(); // QOpenGLBuffer
    pBuffer->create();
    pBuffer->bind();
    pBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    pBuffer->allocate(vertices,vertexCount*16); // numVertices * 4 GLfloats/vertex * 4 bytes/GLfloat = numVertices * 16

    pShaderProgram->bind();
    pShaderProgram->enableAttributeArray("vPosition");
    pShaderProgram->setAttributeBuffer("vPosition",GL_FLOAT,0,4);
    int m_camLocation = pShaderProgram->uniformLocation("pers_cam_xform");
    pShaderProgram->setUniformValue(m_camLocation, perspective*camera*t);
    int colorLocation = pShaderProgram->uniformLocation("icolor");
    pShaderProgram->setUniformValue(colorLocation,renderColor);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDrawArrays(GL_TRIANGLES,0,vertexCount);

    pShaderProgram->disableAttributeArray("vPosition");
    pShaderProgram->release(); // release program

    pBuffer->destroy(); // destroy buffer object
    delete pBuffer; // release buffer object memory

    for (auto &child: children)
    {
        child->render(pShaderProgram,camera,perspective,t, color);
    }
}
