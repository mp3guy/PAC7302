/*
 * ImageProjection.h
 *
 *  Created on: 1 Nov 2011
 *      Author: thomas
 */

#ifndef IMAGEPROJECTION_H_
#define IMAGEPROJECTION_H_

#include <opencv2/opencv.hpp>
#include "OpenGLWidget.h"

class ImageProjection: public OpenGLWidget
{
    public:
        ImageProjection(IplImage * image, float * projection);
        virtual ~ImageProjection();

        static const int GL_WIN_WIDTH = 640;
        static const int GL_WIN_HEIGHT = 480;

    private:
        void draw();

        IplImage * image;

        GLuint cameraImageTextureID;
        GLfloat * projectionMatrix;
};

#endif /* IMAGEPROJECTION_H_ */
