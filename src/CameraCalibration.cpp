/*
 * CameraCalibration.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: thomas
 */

#include "CameraCalibration.h"

CameraCalibration::CameraCalibration(unsigned char * imageData, int numSnaps, string output)
 : n_boards(numSnaps), board_sz(cvSize(Chessboard::board_w, Chessboard::board_h)), outputPrepend(output)
{
    finished = false;

    image_points = cvCreateMat(n_boards * Chessboard::board_total, 2, CV_32FC1);
    object_points = cvCreateMat(n_boards * Chessboard::board_total, 3, CV_32FC1);
    point_counts = cvCreateMat(n_boards, 1, CV_32SC1);
    intrinsic_matrix = cvCreateMat(3, 3, CV_32FC1);
    distortion_coeffs = cvCreateMat(4, 1, CV_32FC1);

    corners = new CvPoint2D32f[Chessboard::board_total];

    successes = 0;
    step = 0;
    corner_count = 0;

    image = cvCreateImageHeader(cvSize(pac7302::WIDTH, pac7302::HEIGHT), IPL_DEPTH_8U, 3);
    image->imageData = (char *)imageData;

    gray_image = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
}

CameraCalibration::~CameraCalibration()
{
    if(image_points != 0)
        cvReleaseMat(&image_points);

    if(object_points != 0)
        cvReleaseMat(&object_points);

    if(point_counts != 0)
        cvReleaseMat(&point_counts);

    cvReleaseMat(&intrinsic_matrix);
    cvReleaseMat(&distortion_coeffs);

    cvReleaseImageHeader(&image);
    cvReleaseImage(&gray_image);

    delete [] corners;
}

void CameraCalibration::calibrationStep()
{
    if(finished)
    {
        cout << "Calibration finished!" << endl;
        return;
    }

    if(successes < n_boards)
    {
        corner_count = Chessboard::board_total;

        int found = cvFindChessboardCorners(image,
                                            board_sz,
                                            corners,
                                            &corner_count,
                                            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

        if(found == 0)
        {
            return;
        }

        cvCvtColor(image, gray_image, CV_RGB2GRAY);

        cvFindCornerSubPix(gray_image,
                           corners,
                           corner_count,
                           cvSize(11, 11),
                           cvSize(-1, -1),
                           cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

        cvDrawChessboardCorners(image, board_sz, corners, corner_count, found);

        if(corner_count == Chessboard::board_total)
        {
            step = successes * Chessboard::board_total;

            for(int i = step, j = 0; j < Chessboard::board_total; ++i, ++j)
            {
                CV_MAT_ELEM(*image_points, float,i,0) = corners[j].x;
                CV_MAT_ELEM(*image_points, float,i,1) = corners[j].y;
                CV_MAT_ELEM(*object_points,float,i,0) = (float) j / Chessboard::board_w;
                CV_MAT_ELEM(*object_points,float,i,1) = (float) (j % Chessboard::board_w);
                CV_MAT_ELEM(*object_points,float,i,2) = 0.0f;
            }

            CV_MAT_ELEM(*point_counts, int,successes,0) = Chessboard::board_total;
            successes++;
        }
    }
    else
    {
        CvMat * object_points2 = cvCreateMat(successes * Chessboard::board_total, 3, CV_32FC1);
        CvMat * image_points2 = cvCreateMat(successes * Chessboard::board_total, 2, CV_32FC1);
        CvMat * point_counts2 = cvCreateMat(successes, 1, CV_32SC1);

        for(int i = 0; i < successes * Chessboard::board_total; ++i)
        {
            CV_MAT_ELEM( *image_points2, float, i, 0) = CV_MAT_ELEM( *image_points, float, i, 0);
            CV_MAT_ELEM( *image_points2, float, i, 1) = CV_MAT_ELEM( *image_points, float, i, 1);
            CV_MAT_ELEM(*object_points2, float, i, 0) = CV_MAT_ELEM( *object_points, float, i, 0);
            CV_MAT_ELEM( *object_points2, float, i, 1) = CV_MAT_ELEM( *object_points, float, i, 1);
            CV_MAT_ELEM( *object_points2, float, i, 2) = CV_MAT_ELEM( *object_points, float, i, 2);
        }

        for(int i = 0; i < successes; ++i)
        {
            CV_MAT_ELEM( *point_counts2, int, i, 0) = CV_MAT_ELEM( *point_counts, int, i, 0);
        }

        cvReleaseMat(&object_points);
        cvReleaseMat(&image_points);
        cvReleaseMat(&point_counts);

        object_points = 0;
        image_points = 0;
        point_counts = 0;

        CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
        CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0f;

        cvCalibrateCamera2(object_points2,
                           image_points2,
                           point_counts2,
                           cvGetSize(image),
                           intrinsic_matrix,
                           distortion_coeffs,
                           NULL,
                           NULL,
                           CV_CALIB_FIX_ASPECT_RATIO);

        string intrinsicsPath = outputPrepend;
        intrinsicsPath.append("intrinsics.xml");

        string distortionPath = outputPrepend;
        distortionPath.append("distortion.xml");

        cvSave(intrinsicsPath.c_str(), intrinsic_matrix);
        cvSave(distortionPath.c_str(), distortion_coeffs);

        cvReleaseMat(&object_points2);
        cvReleaseMat(&image_points2);
        cvReleaseMat(&point_counts2);

        finished = true;
    }
}
