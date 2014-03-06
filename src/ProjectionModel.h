/*
 * ProjectionModel.h
 *
 *  Created on: 1 Nov 2011
 *      Author: thomas
 */

#ifndef PROJECTIONMODEL_H_
#define PROJECTIONMODEL_H_

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "pac7302.h"
#include "Chessboard.h"

using namespace std;

class ProjectionModel
{
    public:
        ProjectionModel(unsigned char * imageData, string path);
        virtual ~ProjectionModel();

        bool loadData();
        void calculateProjection();

        float * getMatrix()
        {
            return (float *)&fullMatrix[0];
        }

    private:
        IplImage * sourceImg;
        IplImage * gray_image;

        string path;

        CvMat * intrinsic_matrix;
        CvMat * distortion_coeffs;
        CvMat * rotation_matrix;
        CvMat * rotation_vector;
        CvMat * translation_vector;
        CvMat * H;
        CvMat * image_points;
        CvMat * object_points;

        float fullMatrix[16];

        CvPoint2D32f * corners;
        CvPoint2D32f objPts[4], imgPts[4];

        CvSize board_sz;
};

#endif /* PROJECTIONMODEL_H_ */
