#include "myopenglwidget.h"
#include <iostream>
#include <rgb.h>

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
{   
    world = new MGL_Node();
    selected = aux = world->end();
    resetCamera();
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    delete world;
}

void MyOpenGLWidget::setMode(Mode::mode mode)
{
    if (mode == Mode::selection)
    {
        selected = aux = world->end();
    }
    m = mode;
}

void MyOpenGLWidget::transformSelected(float factor)
{
    if (selected != world->end())
    {
        switch(m)
        {
        case Mode::triangles:
        case Mode::selection: // do nothing
            break;
        case Mode::translate: (*selected)->translate(factor,axis);
            break;
        case Mode::rotate: (*selected)->rotate(20*factor,axis);
            break;
        case Mode::scale: (*selected)->scaleLinear(factor,axis); // might set a setting for this?
            break;
        case Mode::translateCam:
            if (axis == 0) eye += QVector3D(factor,0.0,0.0);
            else if (axis == 1) eye += QVector3D(0.0,factor,0.0);
            else eye += QVector3D(0.0,0.0,factor);
            calculateCameraMatrix();
            break;
        case Mode::translateTarget:
            if (axis == 0) center += QVector3D(factor,0.0,0.0);
            else if (axis == 1) center += QVector3D(0.0,factor,0.0);
            else center += QVector3D(0.0,0.0,factor);
            calculateCameraMatrix();
            break;
        case Mode::rotateCameraZ:
            break;
        case Mode::nearclip: nearclip += factor;
            if (factor != 0.0) std::cout << nearclip << std::endl;
            resizeGL(width(),height());
            break;
        case Mode::farclip: farclip += factor;
            if (factor != 0.0) std::cout << farclip << std::endl;
            resizeGL(width(),height());
        }
    }
}

void MyOpenGLWidget::newNode()
{
    world->spawnChild();          // add it to the top level of our tree
    selected = --world->end();  // select it
}

void MyOpenGLWidget::deleteSelected()
{
    auto temp = selected;
    MGL_Node *childToDelete = *temp;
    selectNext();
    delete childToDelete;
}

void MyOpenGLWidget::makeParentOf()
{
    if (selected == aux || selected == world->end() || aux == world->end())
        return;

    world->makeXChildOfY(aux,selected);
}

void MyOpenGLWidget::selectParent()
{
    // sets selected to aux, aux is the future child
    if (selected != world->end())
    {
        if (aux == world->end())
        {
            aux = selected;
        }
        else
        {
            // aux is selected, and now we've chosen a parent
            makeParentOf();
            aux = world->end();
        }
    }
}

void MyOpenGLWidget::clearObjects()
{
    std::cout << "clearing objects... ";
    world->deleteChildren();
    selected = aux = world->end();
    std::cout << "objects cleared!" << std::endl;
}

void MyOpenGLWidget::setAxis(int a)
{
    if (a >= 0 && a < 3)
        axis = a;
}

void MyOpenGLWidget::swapAxisOrder(int order)
{
    static int o = 0;
    if (selected != world->end())
    {
        (*selected)->setRotationAxisOrder(o);
        if (o == 0)
            o = 3;
        else
            o = 0;
    }
}

void MyOpenGLWidget::swapTransformOrder(int order)
{
    static int o = 0;
    if (selected != world->end())
    {
        (*selected)->setXformOrder(o);
        if (o == 0)
            o = 1;
        else
            o = 0;
    }
}

void MyOpenGLWidget::selectNext()
{
    if (++selected == world->end())
        selected = world->begin();
}

void MyOpenGLWidget::selectPrevious()
{
    if (selected == world->begin())
        selected = --world->end();
    else
        --selected;
}

void MyOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    // must run after initializeOpenGLFunctions()
    pVao = new QOpenGLVertexArrayObject();
    pVao->create();
    pVao->bind();

    pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    pVertexShader->compileSourceFile(":/glsl/shader.vert");
    std::cout << pVertexShader->log().toLatin1().data(); // print errlog

    pFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    pFragmentShader->compileSourceFile(":/glsl/shader.frag");
    std::cout << pFragmentShader->log().toLatin1().data();// print errlog

    // Must occur after the vertex shader and fragment shader objects
    // are created.
    pShaderProgram = new QOpenGLShaderProgram(this);
    pShaderProgram->create();
    pShaderProgram->addShader(pVertexShader);
    pShaderProgram->addShader(pFragmentShader);
    pShaderProgram->link();


}

void MyOpenGLWidget::paintGL()
{
    float col[] = { 0.0f, 0.5f, 0.8f, 1.0f};
    glClearBufferfv(GL_COLOR,0,col);

    QMatrix4x4 identity;
    for (auto it = world->begin(); it != world->end(); it++)
    {
        if (it == selected)
        {
            renderTree(*it, identity, QVector4D(1.0,0.0,0.0,1.0));
        }
        else if (it == aux)
        {
            renderTree(*it, identity, QVector4D(0.0,0.0,0.25,1.0));
        }
        else
        {
            renderTree(*it, identity, QVector4D(1.0,1.0,1.0,1.0));
        }
    }
}

void MyOpenGLWidget::renderTree(MGL_Node *node, const QMatrix4x4 &transform, const QVector4D &color)
{
    // render triangles in the current object
    int vertexCount = node->getVertexCountLocal();
    GLfloat *vertices = node->getTriangleArray();
    QMatrix4x4 t = transform * node->getXform();

    QOpenGLBuffer *pBuffer = new QOpenGLBuffer();
    pBuffer->create();
    pBuffer->bind();
    pBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    pBuffer->allocate(vertices,vertexCount*16); // numVertices * 4 GLfloats/vertex * 4 bytes/GLfloat = numVertices * 16

    pShaderProgram->bind();
    pShaderProgram->enableAttributeArray("vPosition");
    pShaderProgram->setAttributeBuffer("vPosition",GL_FLOAT,0,4);
    int m_camLocation = pShaderProgram->uniformLocation("pers_cam_xform");
    pShaderProgram->setUniformValue(m_camLocation, matPerspective * matCamera * t);
    int colorLocation = pShaderProgram->uniformLocation("icolor");
    pShaderProgram->setUniformValue(colorLocation,color);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDrawArrays(GL_TRIANGLES,0,vertexCount);

    pShaderProgram->disableAttributeArray("vPosition");
    pShaderProgram->release(); // release program

    pBuffer->destroy(); // destroy buffer object
    delete pBuffer; // release buffer object memory

    for (auto &obj: node->getChildren())
    {
        renderTree(obj, t, color);
    }
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    matPerspective.setToIdentity();// mat is a QMatrix4x4 object
    int fovy = 60;
    matPerspective.perspective(fovy, float(w)/float(h), nearclip,farclip);
//    float *data = matPerspective.data();
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            std::cout << data[i*4+j] << " ";
//        }
//        std::cout << std::endl;
//    }
//    data = matPerspective.inverted().data();
//    std::cout << std::endl;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            std::cout << data[i*4+j] << " ";
//        }
//        std::cout << std::endl;
//    }
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *ev) {
    // store x and y coordinates
    widgetX1 = ev->x();
    widgetY1 = ev->y();
    // width and height are always 600
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent *ev) {
    widgetX2 = ev->x();
    widgetY2 = ev->y();

    // either selects an object or adds a vertex
    if (m == Mode::selection)
    {
        // emit ray and select the nearest visible intersecting object
        Ray r = eyeToPixelRay(widgetX2,widgetY2);

        // detect collision with objects
        int vcount = world->getVertexCountRecursive(), objIndex = 0;
        float *vertices = new float[vcount*3];
        std::vector<int> objIndices;

        for (auto i = world->begin(); i != world->end(); i++)
        {
            objIndices.push_back(objIndex);
            objIndex += 3 * (*i)->fillRaw(vertices+objIndex);
        }

        for (int i = 0; i < vcount; i++)
        {
            std::cout << "vertex " << i << ": " << vertices[i*3] <<
                  " " << vertices[i*3+1] <<
                  " " << vertices[i*3+2] << std::endl;
        }

        auto idx = objIndices.begin();
        float bestT = farclip;
        for (auto node = world->begin(); node != world->end(); node++, idx++)
        {
            float t1 = -1.0, t2 = -1.0;
            std::cout << (*node)->getVertexCountRecursive() << " vcount" << std::endl;
            std::cout << "idx " << *idx << std::endl;
            if (r.intersectsBoundingSphere(vertices+*idx,(*node)->getVertexCountRecursive(),t1,t2))
            {
                std::cout << "t1 " << t1 << " t2 " << t2 << std::endl;
                if (t1 < 0.0 && t2 < 0.0)
                { /* do not select */ }
                else if (t1 < 0.0) // t2 >= 0.0
                {
                    if (t2 < bestT)
                    {
                        bestT = t2;
                        selected = node;
                    }
                }
                else if (t2 < 0.0) // t1 >= 0.0
                {
                    if (t1 < bestT)
                    {
                        bestT = t1;
                        selected = node;
                    }
                }
                else
                {
                    if (t1 < bestT)
                    {
                        bestT = t2 < t1 ? t2 : t1;
                        selected = node;
                    }
                }

            }
            else
            {
                std::cout << "does not intersect" << std::endl;
            }
        }

        std::cout << std::endl;
        delete [] vertices;
    }
    else
    {
        // relatively safe. checks if an object is selected
        setNextVertex();
    }
}


void MyOpenGLWidget::saveRayTracedImage()
{

    for (int y = 0; y < height(); y++)
    {
        for (int x = 0; x < width(); x++)
        {
            // create a ray
            Ray r = eyeToPixelRay(x,y);
            // trace ray


        }
    }
}

void MyOpenGLWidget::convertPixel(double &x, double &y) const
{
    std::cout << "x: " << x << " y: " << y << std::endl;
    x = double(2*x)/width() - 1;
    y = 1 - double(2*y)/height();
    std::cout << "x: " << x << " y: " << y << std::endl;
}

Ray MyOpenGLWidget::eyeToPixelRay(int xInt, int yInt) const
{
    double x = xInt, y = yInt; // convert to floats
    convertPixel(x,y);
    QVector4D p(x,y,0,1);
    p = (matCamera).inverted() * p;
    QVector3D D = (p.toVector3D() - eye).normalized();
    return Ray(eye,D);
}


float MyOpenGLWidget::mouseDistanceY()
{
    return float(widgetY2 - widgetY1) / height();
}

void MyOpenGLWidget::calculateCameraMatrix()
{
    matCamera.setToIdentity();
    matCamera.lookAt(eye,center,up);
//    float *data = matCamera.data();
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            std::cout << data[i*4+j] << " ";
//        }
//        std::cout << std::endl;
//    }
//    data = matCamera.inverted().data();
//    std::cout << std::endl;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            std::cout << data[i*4+j] << " ";
//        }
//        std::cout << std::endl;
//    }
}

void MyOpenGLWidget::resetCamera()
{
    eye = QVector3D(0,0,-1);
    center = QVector3D(0,0,0);
    up = QVector3D(0,1,0);
    calculateCameraMatrix();
}

void MyOpenGLWidget::setNextVertex()
{
    if (selected != world->end())
    {
        if (numVerticesChosen < 3)
        {
            GLfloat x3d = float(2*widgetX2)/width() - 1;
            GLfloat y3d = 1 - float(2*widgetY2)/height();
            QVector4D vertex = QVector4D(x3d,y3d,0,1);
            QMatrix4x4 xform;
            if (selected != world->end()) xform = (*selected)->getXform();
            vertex = (matPerspective * matCamera * xform).inverted() * vertex;
            float x = vertex.x(), y = vertex.y(), z = vertex.z(), w = vertex.w();
            std::cout << "create vertex: " << x << " " << y << " " << z << " " << w << std::endl;
            vertex.setX(x/w); vertex.setY(y/w); vertex.setZ(z/w); vertex.setW(1.0);
            qvertices[numVerticesChosen++] = vertex;
        }
        if (numVerticesChosen == 3)
        {
            (*selected)->addVerticesByVector4D(qvertices, 3);
            numVerticesChosen = 0;
        }
    }
    else
    {
        numVerticesChosen = 0;
    }
}


// TODO: decide if this is useful
bool MyOpenGLWidget::canAddVertex()
{
    return false;
}

void MyOpenGLWidget::genericTriangle()
{
    if (!(world->size() == 0) && selected != world->end())
    {
        QVector4D triangle[3];
        triangle[0] = (matPerspective * matCamera).inverted() * QVector4D(-0.125,0.0,0.0,1.0);
        triangle[1] = (matPerspective * matCamera).inverted() * QVector4D(0.125,0.0,0.0,1.0);
        triangle[2] = (matPerspective * matCamera).inverted() * QVector4D(0.0,0.125,0.0,1.0);

        (*selected)->addVerticesByVector4D(triangle, 3);
    }
}

void MyOpenGLWidget::refresh() {
    update();
}
