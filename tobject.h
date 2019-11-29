#ifndef TOBJECT_H
#define TOBJECT_H

#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <iostream>
#include <vector>
#include "ttransform.h"

struct TObject
{
    static int universal_id;

    TObject();
    ~TObject();

    GLfloat vertices[1200]; // 100 triangles = 300 vertices * 4 floats/vertex = 1200 floats
    int vertexCount = 0; // holds the number of vertices
    const int max_vertices = 300;

    int id; // may delete later
};

#endif // TOBJECT_H
