/*
 * OpenGLWidget.h
 *
 *  Created on: 14 Nov 2010
 *      Author: thomas
 */

#ifndef OPENGLWIDGET_H_
#define OPENGLWIDGET_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <Qt/qgl.h>
#include <qapplication.h>
#include <qimage.h>
#include <iostream>
#include <sys/time.h>
#include <QMouseEvent>
#include <qvector3d.h>
#include <qquaternion.h>
#include <qmatrix4x4.h>
#include <math.h>

#include "ArcBall.h"

using namespace std;

class OpenGLWidget: public QGLWidget
{
	public:
		OpenGLWidget();
		virtual ~OpenGLWidget();

		void initializeGL();
		virtual void paintGL();
		void resizeGL(int width, int height);

	protected:
	    void mousePressEvent(QMouseEvent * event);
	    void mouseMoveEvent(QMouseEvent * event);
	    void mouseReleaseEvent(QMouseEvent * event);
	    void wheelEvent(QWheelEvent * event);
	    virtual void draw();

	    inline void drawCube(const float & scale, const unsigned char & r, const unsigned char & g, const unsigned char & b, bool drawEdges = false)
	    {
	        glBegin(GL_QUADS);
	            glColor3ub(r, g, b);

	            glVertex3f( 1.0f * scale, 1.0f * scale, -1.0f * scale);
	            glVertex3f(-1.0f * scale, 1.0f * scale, -1.0f * scale);
	            glVertex3f(-1.0f * scale, 1.0f * scale, 1.0f * scale);
	            glVertex3f( 1.0f * scale, 1.0f * scale, 1.0f * scale);

	            glVertex3f( 1.0f * scale, -1.0f * scale, 1.0f * scale);
	            glVertex3f(-1.0f * scale, -1.0f * scale, 1.0f * scale);
	            glVertex3f(-1.0f * scale, -1.0f * scale, -1.0f * scale);
	            glVertex3f( 1.0f * scale, -1.0f * scale, -1.0f * scale);

	            glVertex3f( 1.0f * scale, 1.0f * scale, 1.0f * scale);
	            glVertex3f(-1.0f * scale, 1.0f * scale, 1.0f * scale);
	            glVertex3f(-1.0f * scale, -1.0f * scale, 1.0f * scale);
	            glVertex3f( 1.0f * scale, -1.0f * scale, 1.0f * scale);

	            glVertex3f( 1.0f * scale, -1.0f * scale, -1.0f * scale);
	            glVertex3f(-1.0f * scale, -1.0f * scale, -1.0f * scale);
	            glVertex3f(-1.0f * scale, 1.0f * scale, -1.0f * scale);
	            glVertex3f( 1.0f * scale, 1.0f * scale, -1.0f * scale);

	            glVertex3f(-1.0f * scale, 1.0f * scale, 1.0f * scale);
	            glVertex3f(-1.0f * scale, 1.0f * scale, -1.0f * scale);
	            glVertex3f(-1.0f * scale, -1.0f * scale, -1.0f * scale);
	            glVertex3f(-1.0f * scale, -1.0f * scale, 1.0f * scale);

	            glVertex3f(1.0f * scale, 1.0f * scale, -1.0f * scale);
	            glVertex3f(1.0f * scale, 1.0f * scale, 1.0f * scale);
	            glVertex3f(1.0f * scale, -1.0f * scale, 1.0f * scale);
	            glVertex3f(1.0f * scale, -1.0f * scale, -1.0f * scale);
	        glEnd();

	        if(drawEdges)
	        {
	            glBegin(GL_LINES);
                    glColor3ub(255, 255, 255);

                    glVertex3f(scale, -scale, -scale);
                    glVertex3f(-scale, -scale, -scale);

                    glVertex3f(scale, -scale, scale);
                    glVertex3f(-scale, -scale, scale);

                    glVertex3f(scale, scale, -scale);
                    glVertex3f(-scale, scale, -scale);

                    glVertex3f(scale, scale, scale);
                    glVertex3f(-scale, scale, scale);

                    glVertex3f(scale, -scale, -scale);
                    glVertex3f(scale, -scale, scale);

                    glVertex3f(-scale, scale, scale);
                    glVertex3f(-scale, scale, -scale);

                    glVertex3f(scale, scale, -scale);
                    glVertex3f(scale, scale, scale);

                    glVertex3f(-scale, -scale, scale);
                    glVertex3f(-scale, -scale, -scale);

                    glVertex3f(scale, scale, -scale);
                    glVertex3f(scale, -scale, -scale);

                    glVertex3f(-scale, -scale, -scale);
                    glVertex3f(-scale, scale, -scale);

                    glVertex3f(scale, scale, scale);
                    glVertex3f(scale, -scale, scale);

                    glVertex3f(-scale, scale, scale);
                    glVertex3f(-scale, -scale, scale);
                glEnd();
	        }
	    }

	    inline void drawCubeEdges(const float & size, const unsigned char & r, const unsigned char & g, const unsigned char & b)
	    {
	        glBegin(GL_LINES);
	            glColor3ub(r, g, b);

	            glVertex3f(size, -size, -size);
	            glVertex3f(-size, -size, -size);

	            glVertex3f(size, -size, size);
	            glVertex3f(-size, -size, size);

	            glVertex3f(size, size, -size);
	            glVertex3f(-size, size, -size);

	            glVertex3f(size, size, size);
	            glVertex3f(-size, size, size);

	            glVertex3f(size, -size, -size);
	            glVertex3f(size, -size, size);

	            glVertex3f(-size, size, size);
	            glVertex3f(-size, size, -size);

	            glVertex3f(size, size, -size);
	            glVertex3f(size, size, size);

	            glVertex3f(-size, -size, size);
	            glVertex3f(-size, -size, -size);

	            glVertex3f(size, size, -size);
	            glVertex3f(size, -size, -size);

	            glVertex3f(-size, -size, -size);
	            glVertex3f(-size, size, -size);

	            glVertex3f(size, size, size);
	            glVertex3f(size, -size, size);

	            glVertex3f(-size, size, size);
	            glVertex3f(-size, -size, size);
	        glEnd();
	    }

	    int lastMouseX, lastMouseY;
	    float cameraZoom;

	    ArcBall * myArcBall;
};

#endif /* OPENGLWIDGET_H_ */
