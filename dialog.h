#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_clearButton_clicked();

    void on_newObjectButton_clicked();

    void on_selectObjectButton_clicked();

    void on_selectNextButton_clicked();

    void on_selectPreviousButton_clicked();

    void on_translateObjectButton_clicked();

    void on_parentButton_clicked();

    void on_rotateButton_clicked();

    void on_scaleButton_clicked();

    void on_xAxisRadioButton_toggled(bool checked);

    void on_yAxisRadioButton_toggled(bool checked);

    void on_zAxisRadioButton_toggled(bool checked);

    void on_swapXformButton_clicked();

    void on_swapAxisButton_clicked();

    void on_translateCameraButton_clicked();

    void on_translateTargetButton_clicked();

    void on_rotateCameraZButton_clicked();

    void on_nearPlaneButton_clicked();

    void on_farPlaneButton_clicked();

    void on_resetCameraButton_clicked();

    void on_addTriangle_clicked();

    void refresh();
private:
    Ui::Dialog *ui;
    QTimer *timer60fps;
    unsigned short movement_keys;
};
#endif // DIALOG_H
