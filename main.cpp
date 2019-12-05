#include "dialog.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <raytracer.h>

int main(int argc, char *argv[])
{
    // for mac osx compatibility
    static QSurfaceFormat mSurfaceFormat;
    mSurfaceFormat.setDepthBufferSize(24);
    mSurfaceFormat.setStencilBufferSize(8);
    mSurfaceFormat.setVersion(4,0);
    mSurfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(mSurfaceFormat);

    QApplication a(argc, argv);
    Dialog w;
    w.show();
    return a.exec();
}
