#ifndef GLDISPLAYWIDGET_H
#define GLDISPLAYWIDGET_H

#include <QGLWidget> // Module QtOpengL (classes QGLxxx in Qt4),
                     // with widget and rendering classes descending from QGLWidget,
                     // deprecated for module QtGui (classes QOpenGLxxx )
                     // and module widgets (QWidget, QGraphicsView).
#include <QtWidgets>
#include <QTimer>
#include "mesh.h"  // Model

class GLDisplayWidget : public QGLWidget
{
public:
    explicit GLDisplayWidget(QWidget *parent = 0);

    void initializeGL(); // The scene may be initialized in this function since the GeometricWorld is a data member...
    void paintGL(); // Display the scene Gl
    void resizeGL(int width, int height);

    GeometricWorld& get_world() {return _geomWorld;}


protected:
    // Mouse Management
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    QTimer _timer; // To update the scene
    float _X = 0, _Y = 0 ,_Z = 0; // Translation
    float _angle_x; // Rotation

    QPoint _lastPosMouse; // To keep the last position of the mouse

    GeometricWorld _geomWorld; // The scene to be displayed
};

#endif // GLDISPLAYWIDGET_H