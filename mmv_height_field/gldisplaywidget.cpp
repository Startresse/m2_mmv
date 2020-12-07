#include "gldisplaywidget.h"
#ifdef __APPLE__
    #include <glu.h>
#else
    #include <GL/glu.h>
#endif

#include "QDebug"

GLDisplayWidget::GLDisplayWidget(QWidget *parent) : QGLWidget(parent)
{
    // Update the scene
    connect( &_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    _timer.start(16); // Starts or restarts the timer with a timeout interval of 16 milliseconds.
}

void GLDisplayWidget::initializeGL()
{
    // background color
    glClearColor(0.2, 0.2, 0.2, 1);

    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

void GLDisplayWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Center the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, distance,
              0, 0, 0,
              0, 1, 0);

    // transform
    glRotatef(x_rot / 36.0f, 1.0f , 0.0f, 0.0f );
    glRotatef(y_rot / 36.0f, 0.0f , 1.0f, 0.0f );
    glRotatef(z_rot / 36.0f, 0.0f , 0.0f, 1.0f );

    _geomWorld.draw();
}

void GLDisplayWidget::resizeGL(int width, int height){

    glViewport(0, 0, width, height); //Viewport in the world window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    updateGL();
}

// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void GLDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL )
        _lastPosMouse = event->pos();
}

// Mouse movement management
void GLDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - _lastPosMouse.x();
    int dy = event->y() - _lastPosMouse.y();

    if( event != NULL )
    {
        rotateBy(dy*8.0, 0     , 0);
        rotateBy(0     , dx*8.0, 0);

        updateGL();
    }
    _lastPosMouse = event->pos();
}

// Mouse Management for the zoom
void GLDisplayWidget::wheelEvent(QWheelEvent *event) {
    distance *= 1.0 + (-1.0 * event->delta() / 12000.0);
}
void GLDisplayWidget::rotateBy(float x, float y, float z)
{
    x_rot += x;
    y_rot += y;
    z_rot += z;
}
