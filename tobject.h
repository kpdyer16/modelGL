#ifndef TOBJECT_H
#define TOBJECT_H

#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <iostream>
#include <vector>
#include "ttransform.h"

class TObject
{
public:
    TObject();
    TObject(TObject *parent);
    ~TObject();

    std::string name;

    std::list<TObject *> children;
    std::list<TObject *>::iterator begin();
    std::list<TObject *>::iterator end();

    void setTransformOrder(int order);
    void setRotationAxisOrder(int order);

    void swapTransformOrder();
    void swapAxisOrder();

    void addChild(TObject *child);
    void removeChild(std::list<TObject *>::iterator &childIter);
    void deleteChild(TObject *child);

    void deleteChildren();

    QMatrix4x4 getxform() { return transform.transformMatrix(); }


    void addTriangle(QVector4D*); // 3 QVector4D
    void addTriangles(GLfloat*, int); // less safe
    void clearTriangles();

    void translate(float, int);
    void rotate(float, int);
    void scale(float, int);

    void render(QOpenGLShaderProgram*,const QMatrix4x4 &perspective,
                const QMatrix4x4 &camera, const QMatrix4x4 &, int) const;

    // debug
    bool hasParent() { std::cout << "parent: " << parent << std::endl; return parent != nullptr; }
protected:
//    void removeChild(int index);


private:
    TTransform transform;
    GLfloat vertices[300][4]; // 300 vertices = 100 triangles
    int vertexCount = 0; // holds the number of vertices

    TObject *parent;

    const static QVector4D white, red, blue;
};

#endif // TOBJECT_H
