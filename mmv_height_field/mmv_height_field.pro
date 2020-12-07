QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    box2.cpp \
    gldisplaywidget.cpp \
    grid2.cpp \
    heightfield.cpp \
    main.cpp \
    mainwindow.cpp \
    mesh.cpp \
    scalarpoint2.cpp \
    sf2.cpp

HEADERS += \
    box2.h \
    gldisplaywidget.h \
    grid2.h \
    heightfield.h \
    mainwindow.h \
    mesh.h \
    scalarpoint2.h \
    sf2.h \
    ui_mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += \
    /usr/local/include/opencv4

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += \
    -lGLU \
    -L/usr/local/lib -L/home/mike/opencv-4.0.1/build -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_shape -lopencv_videoio -lopencv_calib3d -lopencv_face
