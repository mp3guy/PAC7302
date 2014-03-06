/*
 * OpenGLWidget.cpp
 *
 *  Created on: 14 Nov 2010
 *      Author: thomas
 */

#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget()
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::SampleBuffers));
    lastMouseX = width();
    lastMouseY = height();
    myArcBall = new ArcBall((GLint)width(), (GLint)height());
    cameraZoom = -400.0f;
}

OpenGLWidget::~OpenGLWidget()
{
    delete myArcBall;
}

void OpenGLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void OpenGLWidget::draw()
{

}

void OpenGLWidget::resizeGL(int width, int height)
{
    myArcBall->setBounds(width, height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat x = 0.5f * (GLfloat(width) / GLfloat(height));
    glFrustum(-x, x, -0.5, 0.5, 1.0, 10240.0);

    glMatrixMode(GL_MODELVIEW);
}

void OpenGLWidget::mousePressEvent(QMouseEvent * event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        lastMouseX = event->x();
        lastMouseY = event->y();
        myArcBall->click(lastMouseX, lastMouseY);
        updateGL();
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent * event)
{
    if(event->type() == QEvent::MouseMove &&
      (event->x() != lastMouseX || event->y() != lastMouseY))
    {
        myArcBall->drag(lastMouseX, lastMouseY);
        lastMouseX = event->x();
        lastMouseY = event->y();
        updateGL();
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->type() == QEvent::MouseButtonRelease)
    {
        lastMouseX = event->x();
        lastMouseY = event->y();
        updateGL();
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent * event)
{
    if(event->type() == QEvent::Wheel)
    {
        if(event->delta() > 0)
        {
            cameraZoom += 80;
        }
        else
        {
            cameraZoom -= 80;
        }
        updateGL();
    }
}
