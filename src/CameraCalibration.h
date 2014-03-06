/*
 * CameraCalibration.h
 *
 *  Created on: 31 Oct 2011
 *      Author: thomas
 */

#ifndef CAMERACALIBRATION_H_
#define CAMERACALIBRATION_H_

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "pac7302.h"
#include "Chessboard.h"

using namespace std;

class CameraCalibration
{
    public:
        CameraCalibration(unsigned char * imageData, int numSnaps, string output);
        virtual ~CameraCalibration();

        bool isDone()
        {
            return finished;
        }

        void calibrationStep();

    private:
        const int n_boards;

        CvSize board_sz;
        CvMat * image_points;
        CvMat * object_points;
        CvMat * point_counts;
        CvMat * intrinsic_matrix;
        CvMat * distortion_coeffs;
        CvPoint2D32f * corners;

        IplImage * image;
        IplImage * gray_image;

        string outputPrepend;

        int corner_count;
        int successes;
        int step;

        bool finished;
};

#endif /* CAMERACALIBRATION_H_ */
