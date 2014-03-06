/*
 * ImageProjection.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: thomas
 */

#include "ImageProjection.h"

ImageProjection::ImageProjection(IplImage * image, float * projection)
 : image(image), projectionMatrix((GLfloat *)projection)
{
    glGenTextures(1, &cameraImageTextureID);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, cameraImageTextureID);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

ImageProjection::~ImageProjection()
{
    glDeleteTextures(1, &cameraImageTextureID);
}

void ImageProjection::draw()
{
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -6.0f + cameraZoom);

    glPushMatrix();

    glMultMatrixf(myArcBall->getTransform());

    glRotatef(180.0, 1.0, 0.0, 0.0);

    glEnable(GL_TEXTURE_RECTANGLE_ARB);

    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);

    float scale = 150.0f;

    glPushMatrix();

    glMultMatrixf(projectionMatrix);

    float ratio = (float)image->width / (float)image->height;

    glBegin(GL_QUADS);
        glTexCoord2i(image->width, image->height);
        glVertex3f(-scale * ratio, -scale, scale);

        glTexCoord2i(0, image->height);
        glVertex3f(scale, -scale, scale);

        glTexCoord2i(0, 0);
        glVertex3f(scale, scale, scale);

        glTexCoord2i(image->width, 0);
        glVertex3f(-scale * ratio, scale, scale);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_RECTANGLE_ARB);

    drawCube(16, 175, 175, 175, true);

    glPopMatrix();
}
