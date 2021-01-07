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

    void activate_mouse_record() {record_mouse_click = FIRST;}

    GeometricWorld& get_world() {return _geomWorld;}


protected:
    // Mouse Management
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void rotateBy(float x, float y, float z);

private:

    QTimer _timer; // To update the scene
    float distance = 5.0;
    float x_rot = 0, y_rot = 0, z_rot = 0;

    enum get_cursor_twice {DONE, FIRST, SECOND};
    get_cursor_twice record_mouse_click = DONE;
    QPointF relative_point(const QPoint&);
    QPointF click0, click1;

    QPoint _lastPosMouse; // To keep the last position of the mouse

    GeometricWorld _geomWorld; // The scene to be displayed
};

#endif // GLDISPLAYWIDGET_H
