#include "dialog.h"
#include "ui_dialog.h"
#include <QTimer>
#include <mode.h>

// **************************
// Defines for movement_keys
// **************************
#define W_KEY   0x00000001
#define A_KEY   0x00000002
#define S_KEY   0x00000004
#define D_KEY   0x00000008
#define SB_KEY  0x00000010
#define SH_KEY  0x00000020


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->openGLWidget->setStatusBar(ui->label);
    setFocusPolicy(Qt::ClickFocus);
    setFocus();
    timer60fps = new QTimer();
    if (timer60fps != nullptr) {
        timer60fps->setInterval(1000.0 / 60.0);
        connect(timer60fps, &QTimer::timeout, this, &Dialog::refresh);
        timer60fps->start();
    }
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_clearButton_clicked()
{
    ui->openGLWidget->clearObjects();
    ui->openGLWidget->setMode(Mode::triangles);
    ui->label->setText("Objects cleared!");
}

void Dialog::on_newObjectButton_clicked()
{
    ui->openGLWidget->newNode();
    ui->label->setText("New node generated");
}

void Dialog::on_selectObjectButton_clicked()
{
    // TODO: redo this function
    ui->label->setText("Click an object to select it");
    ui->openGLWidget->setMode(Mode::selection);
}

void Dialog::on_selectNextButton_clicked()
{
    ui->openGLWidget->selectNext();
}

void Dialog::on_selectPreviousButton_clicked()
{
    ui->openGLWidget->selectPrevious();
}

void Dialog::on_translateObjectButton_clicked()
{
    // TODO: redo this function
    ui->openGLWidget->setMode(Mode::translate);
}

void Dialog::on_parentButton_clicked()
{
    ui->openGLWidget->selectParent();
}

void Dialog::on_rotateButton_clicked()
{
    // TODO: redo this function
    ui->openGLWidget->setMode(Mode::rotate);
}

void Dialog::on_scaleButton_clicked()
{
    // TODO: redo this function
    ui->openGLWidget->setMode(Mode::scale);
}

void Dialog::on_xAxisRadioButton_toggled(bool checked)
{
    if (checked)
    {
        ui->openGLWidget->setAxis(0);
    }
}

void Dialog::on_yAxisRadioButton_toggled(bool checked)
{
    if (checked)
    {
        ui->openGLWidget->setAxis(1);
    }
}

void Dialog::on_zAxisRadioButton_toggled(bool checked)
{
    if (checked)
    {
        ui->openGLWidget->setAxis(2);
    }
}

void Dialog::on_swapXformButton_clicked()
{
    ui->openGLWidget->swapTransformOrder(0);
}

void Dialog::on_swapAxisButton_clicked()
{
    ui->openGLWidget->swapAxisOrder(0);
}

void Dialog::on_translateCameraButton_clicked()
{
    // TODO: redo this function
    ui->openGLWidget->setMode(Mode::translateCam);
}

void Dialog::on_translateTargetButton_clicked()
{
    // TODO: redo this function
    ui->openGLWidget->setMode(Mode::translateTarget);
}

void Dialog::on_rotateCameraZButton_clicked()
{
    // TODO: redo this function
    ui->openGLWidget->setMode(Mode::rotateCameraZ);
}

void Dialog::on_nearPlaneButton_clicked()
{
    // TODO: redo this function
    ui->openGLWidget->setMode(Mode::nearclip);
}

void Dialog::on_farPlaneButton_clicked()
{
    // TODO: redo this function
    ui->openGLWidget->setMode(Mode::farclip);
}

void Dialog::on_resetCameraButton_clicked()
{
    ui->openGLWidget->resetCamera();
}

void Dialog::on_addTriangle_clicked()
{
    ui->openGLWidget->genericTriangle();
}

void Dialog::keyPressEvent(QKeyEvent *ev)
{
    QWidget::keyPressEvent(ev);
    if (!ev->isAutoRepeat()) {
        switch(ev->key()) {
        case 'W': movement_keys |= W_KEY;
            break;
        case 'A': movement_keys |= A_KEY;
            break;
        case 'S': movement_keys |= S_KEY;
            break;
        case 'D': movement_keys |= D_KEY;
            break;
//        case 'I': ui->openGLWidget->setMode(Mode::);
//            break;
//        case 'O': ui->openGLWidget->setMode(1);
//            break;
//        case 'P': ui->openGLWidget->setMode(2);
//            break;
        case 32: movement_keys |= SB_KEY;
            break;
        case 16777248: movement_keys |= SH_KEY;
        }
    }
}

void Dialog::keyReleaseEvent(QKeyEvent *ev)
{
    QWidget::keyReleaseEvent(ev);
    if (!ev->isAutoRepeat()) {
        switch(ev->key()) {
        case 'W': movement_keys &= ~W_KEY;
            break;
        case 'A': movement_keys &= ~A_KEY;
            break;
        case 'S': movement_keys &= ~S_KEY;
            break;
        case 'D': movement_keys &= ~D_KEY;
            break;
//        case 'I': ui->openGLWidget->setMode(Mode::);
//            break;
//        case 'O': ui->openGLWidget->setMode(1);
//            break;
//        case 'P': ui->openGLWidget->setMode(2);
//            break;
        case 32: movement_keys &= ~SB_KEY;
            break;
        case 16777248: movement_keys &= ~SH_KEY;
        }
    }
}

void Dialog::refresh()
{
    static float d = 0.03125;
    float factor = 0.0;
    if (movement_keys & W_KEY) factor += d;
    if (movement_keys & S_KEY) factor -= d;

    ui->openGLWidget->transformSelected(factor);
    ui->openGLWidget->refresh();
}
