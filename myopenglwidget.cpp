#include "myopenglwidget.h"
#include <iostream>

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

void MyOpenGLWidget::activateKeyboard(bool activate)
{
    keyboardActivated = activate;
    mouseMode = MModes::Neutral;
    keyboardMode = KModes::Neutral;
}

void MyOpenGLWidget::setKeyboardMode(int m)
{
    if (m == KModes::Object)
    {
        colorSetting = true;
        keyboardMode = m;
    }
    else if (m > KModes::Object && m <= KModes::FarPlane)
    {
        colorSetting = false;
        keyboardMode = m;
    }
    else
    {
        colorSetting = false;
        keyboardMode = KModes::Neutral;
    }
}

void MyOpenGLWidget::setMouseMode(int m)
{
    if (m > MModes::Neutral && m < MModes::CameraTranslate)
    {
        colorSetting = true;
        mouseMode = m;
    }
    else if (m <= MModes::FarPlane)
    {
        colorSetting = false;
        mouseMode = m;
    }
    else
    {
        colorSetting = false;
        mouseMode = MModes::Neutral;
    }
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
            o = 1;
        else
            o = 0;
    }
//    if (!keyboardActivated)
//    {
//        if (mouseMode > MModes::Neutral && mouseMode < MModes::CameraTranslate)
//            (*selected)->setRotationAxisOrder();
//    }
//    else
//    {
//        if (keyboardMode == KModes::Object)
//            (*selected)->setRotationAxisOrder();
//    }
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
//    if (!keyboardActivated)
//    {
//        if (mouseMode > MModes::Neutral && mouseMode < MModes::CameraTranslate)
//            (*selected)->swapAxisOrder();
//    }
//    else
//    {
//        if (keyboardMode == KModes::Object)
//            (*selected)->swapAxisOrder();
//    }
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

    /*
    QMatrix4x4 identity;
    for (auto object: world->children)
    {
        int color = 0;
        if (object == *selected)
        {
            color = colorSetting ? 1 : 0;
        }
        else if (object == *aux)
        {
            color = colorSetting ? 2 : 0;
        }
        else
        {
//            std::cout << "Not selected\nobject: " << object
//                      << "selected: " << *selected << std::endl;
        }
        object->render(pShaderProgram,matPerspective,matCamera,identity,color);
    } */
}

void MyOpenGLWidget::renderTree(MGL_Node *node, const QMatrix4x4 &transform, const QVector4D &color)
{
    // render triangles in the current object
    int vertexCount = node->getVertexCount();
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
    matPerspective.perspective(fovy, GLfloat(w)/GLfloat(h), nearclip,farclip);
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *ev) {
    // store x and y coordinates
    widgetX1 = ev->x();
    widgetY1 = ev->y();
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent *ev) {
    widgetX2 = ev->x();
    widgetY2 = ev->y();
    setNextVertex();
}

/*
 * MOUSE CONTROLS
 * KEYBOARD IS INACTIVE
*/

float MyOpenGLWidget::mouseDistanceY()
{
    return float(widgetY2 - widgetY1) / height();
}

void MyOpenGLWidget::mouseTranslateObject()
{
    std::cout << "axis: " << axis << "\ndistance: " << mouseDistanceY();
//    (*selected)->translate(mouseDistanceY()*2,axis);
}
void MyOpenGLWidget::mouseRotateObject()
{
//    (*selected)->rotate(mouseDistanceY()*10,axis);
}

void MyOpenGLWidget::mouseScaleObject()
{
//    (*selected)->scale(mouseDistanceY()*3,axis);
}

void MyOpenGLWidget::mouseTranslateCamera()
{
    switch(axis)
    {
    case 0: eye += QVector3D(mouseDistanceY()*3.0,0,0);
        break;
    case 1: eye += QVector3D(0,mouseDistanceY()*3.0,0);
        break;
    case 2: eye += QVector3D(0,0,mouseDistanceY()*3.0);
        break;
    }
    calculateCameraMatrix();
}

void MyOpenGLWidget::mouseTranslateTarget()
{
    switch(axis)
    {
    case 0: center += QVector3D(mouseDistanceY()*3.0,0,0);
        break;
    case 1: center += QVector3D(0,mouseDistanceY()*3.0,0);
        break;
    case 2: center += QVector3D(0,0,mouseDistanceY()*3.0);
        break;
    }
    calculateCameraMatrix();
}

void MyOpenGLWidget::mouseRotateCameraZ()
{

}

void MyOpenGLWidget::mouseModifyNearPlane()
{
    nearclip += (mouseDistanceY()*10);
    std::cout << nearclip << std::endl;
}

void MyOpenGLWidget::mouseModifyFarPlane()
{
    farclip += mouseDistanceY();
}

void MyOpenGLWidget::calculateCameraMatrix()
{
    matCamera.setToIdentity();
    matCamera.lookAt(eye,center,up);
}

void MyOpenGLWidget::resetCamera()
{
    eye = QVector3D(0,0,-10);
    center = QVector3D(0,0,0);
    up = QVector3D(0,1,0);
    calculateCameraMatrix();
}


void MyOpenGLWidget::testTriangle()
{
}

void MyOpenGLWidget::setNextVertex()
{
    if (numVerticesChosen < 3)
    {
        GLfloat x3d = float(2*widgetX2)/width() - 1;
        GLfloat y3d = 1 - float(2*widgetY2)/height();
        QVector4D vertex = QVector4D(x3d,y3d,0,1);
        QMatrix4x4 xform;
        if (selected != world->end()) xform = (*selected)->getXform();
        vertex = (matPerspective * matCamera * xform).inverted() * vertex;
        qvertices[numVerticesChosen++] = vertex;
    }
    if (numVerticesChosen == 3)
    {
        if (selected != world->end())
        {
            (*selected)->addVerticesByVector4D(qvertices, 3);
            numVerticesChosen = 0;
        }
    }
}


bool MyOpenGLWidget::canAddVertex()
{
    if (keyboardActivated)
        return keyboardMode == KModes::Object;
    else
        return mouseMode == MModes::Select;
}

void MyOpenGLWidget::addTriangle()
{

}

void MyOpenGLWidget::genericTriangle()
{
//    if (!world->children.empty() && selected != world->end())
//    {
//        QVector4D triangle[3];
//        triangle[0] = QVector4D(-1.0,0.0,0.0,1.0);
//        triangle[1] = QVector4D(1.0,0.0,0.0,1.0);
//        triangle[2] = QVector4D(0.0,1.0,0.0,1.0);
//        (*selected)->addTriangle(triangle);

//        newTObject();
//        triangle[0] = QVector4D(0.0,0.5,0.0,1.0);
//        triangle[1] = QVector4D(1.5,0.5,0.0,1.0);
//        triangle[2] = QVector4D(0.0,-1.5,0.0,1.0);
//        (*selected)->addTriangle(triangle);
//    }
}

void MyOpenGLWidget::refresh() {
    update();
}
