#include "dialog.h"
#include "ui_dialog.h"
#include <QTimer>

// **************************
// Defines for movement_keys
// **************************
#define W_PRESSED   0x0008
#define A_PRESSED   0x0004
#define S_PRESSED   0x0002
#define D_PRESSED   0x0001
#define SB_PRESSED  0x0020
#define SH_PRESSED  0x0010
#define W_RELEASED  0x00F7
#define A_RELEASED  0x00FB
#define S_RELEASED  0x00FD
#define D_RELEASED  0x00FE
#define SB_RELEASED 0x00DF
#define SH_RELEASED 0x00EF



Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
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
}

void Dialog::on_parentButton_clicked()
{
    ui->openGLWidget->selectParent();
}

void Dialog::on_rotateButton_clicked()
{
    // TODO: redo this function
}

void Dialog::on_scaleButton_clicked()
{
    // TODO: redo this function
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
}

void Dialog::on_translateTargetButton_clicked()
{
    // TODO: redo this function
}

void Dialog::on_rotateCameraZButton_clicked()
{
    // TODO: redo this function
}

void Dialog::on_nearPlaneButton_clicked()
{
    // TODO: redo this function
}

void Dialog::on_farPlaneButton_clicked()
{
    // TODO: redo this function
}

void Dialog::on_resetCameraButton_clicked()
{
    ui->openGLWidget->resetCamera();
}

void Dialog::on_addTriangle_clicked()
{
    ui->openGLWidget->genericTriangle();
}

void Dialog::refresh()
{
    ui->openGLWidget->refresh();
}
