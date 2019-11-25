#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
#include <QSurfaceFormat>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QWidget>
#include <iostream>
#include "tobject.h"
#include "mode.h"

class MyOpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions_4_0_Core
{
    Q_OBJECT
public:
    MyOpenGLWidget(QWidget *parent=nullptr);
    ~MyOpenGLWidget() override;

    void refresh();


    void newTObject(); // creates new TObject and selects it
    void deleteSelected();
    void makeParentOf();
    void clearObjects();

    void activateKeyboard(bool);
    bool keyboardIsActivated() {return keyboardActivated;}
    void setMouseMode(int mode);
    void setKeyboardMode(int mode);

    void setAxis(int axis);
    void swapTransformOrder();
    void swapAxisOrder();
    void resetCamera();
    void selectParent();

    void selectNext();
    void selectPrevious();

    // interface for transforming the world
    void genericTriangle();
    void translateWorld(float,float,float);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;


    // uses saved mouse click values
    void setNextVertex();        // add a point

    // MOUSE CONTROLS
    void mouseTranslateObject(); // operates on selected object
    void mouseRotateObject();    // ""
    void mouseScaleObject();     // ""
    void mouseTranslateCamera(); // translate eye position
    void mouseTranslateTarget(); // translate center
    void mouseRotateCameraZ();   // rotate up vector
    void mouseModifyNearPlane();
    void mouseModifyFarPlane();

    float mouseDistanceY();
    void calculateCameraMatrix();

private:



    //********************************************
    //********************************************
    //********************************************

    QMatrix4x4 matCamera, matPerspective;
    QVector3D eye, center, up;
    int nearclip = 1, farclip = 20;

    // empty TObject to manage user-created objects
    // all newly created objects are children of worldObject
    TObject *world;
    std::list<TObject *>::iterator selected, aux;

    // essentially a mutex for UI controls
    bool keyboardActivated = false;

    // when true, selected is red and aux (if it points to a child) is dark blue
    bool colorSetting = false;


    int mouseMode = 0;
    int keyboardMode = 0;

    // rendering
    QOpenGLVertexArrayObject *pVao;
    QOpenGLBuffer *pBuffer;
    QOpenGLShader *pVertexShader, *pFragmentShader;
    QOpenGLShaderProgram *pShaderProgram;

    int widgetX1,widgetY1,     // mousePressEvent coordinates
        widgetX2,widgetY2;     // mouseReleaseEvent coordinates

    // UI-related components
    int viewportCoordinatesX[3], viewportCoordinatesY[3]; // holds each each vertex for the next triangle to be drawn
    QVector4D qvertices[3];
    int numVerticesChosen = 0;
    int axis = 0;

    // helper functions
    void addTriangle();
    void testTriangle();
    bool canAddVertex();
};

#endif // MYOPENGLWIDGET_H
