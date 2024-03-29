QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    light.cpp \
    main.cpp \
    dialog.cpp \
    mgl_node.cpp \
    material.cpp \
    mgl_parser.cpp \
    myopenglwidget.cpp \
    sobject.cpp \
    tobject.cpp \
    ttransform.cpp

HEADERS += \
    dialog.h \
    mgl_node.h \
    lexer.h \
    light.h \
    material.h \
    mgl_objectbuffer.h \
    mgl_parser.h \
    mode.h \
    myopenglwidget.h \
    rgb.h \
    sobject.h \
    tobject.h \
    ttransform.h

FORMS += \
    dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shader.frag \
    shader.vert

RESOURCES += \
    res.qrc
