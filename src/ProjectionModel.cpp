/*
 * ProjectionModel.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: thomas
 */

#include "ProjectionModel.h"

ProjectionModel::ProjectionModel(unsigned char * imageData, string path)
{
    this->path = path;

    sourceImg = cvCreateImageHeader(cvSize(pac7302::WIDTH, pac7302::HEIGHT), IPL_DEPTH_8U, 3);
    sourceImg->imageData = (char *)imageData;

    gray_image = cvCreateImage(cvSize(sourceImg->width, sourceImg->height), IPL_DEPTH_8U, 1);

    rotation_matrix = cvCreateMat(3, 3, CV_32F);
    rotation_vector = cvCreateMat(3, 1, CV_32F);
    translation_vector = cvCreateMat(3, 1, CV_32F);

    H = cvCreateMat(3, 3, CV_32F);
    image_points  = cvCreateMat(4, 1, CV_32FC2);
    object_points = cvCreateMat(4, 1, CV_32FC3);

    corners = new CvPoint2D32f[Chessboard::board_total];

    board_sz = cvSize(Chessboard::board_w, Chessboard::board_h);
}

ProjectionModel::~ProjectionModel()
{
    cvReleaseImageHeader(&sourceImg);
    cvReleaseImage(&gray_image);

    if(intrinsic_matrix != 0)
    {
        cvReleaseMat(&intrinsic_matrix);
    }

    if(distortion_coeffs != 0)
    {
        cvReleaseMat(&distortion_coeffs);
    }

    cvReleaseMat(&rotation_matrix);
    cvReleaseMat(&rotation_vector);
    cvReleaseMat(&translation_vector);
    cvReleaseMat(&H);
    cvReleaseMat(&image_points);
    cvReleaseMat(&object_points);

    delete [] corners;
}

bool ProjectionModel::loadData()
{
    if(intrinsic_matrix != 0)
    {
        cvReleaseMat(&intrinsic_matrix);
    }

    if(distortion_coeffs != 0)
    {
        cvReleaseMat(&distortion_coeffs);
    }

    string intrinsicsPath = path;
    intrinsicsPath.append("intrinsics.xml");

    string distortionPath = path;
    distortionPath.append("distortion.xml");

    intrinsic_matrix = (CvMat*)cvLoad(intrinsicsPath.c_str());
    distortion_coeffs = (CvMat*)cvLoad(distortionPath.c_str());

    if(intrinsic_matrix == 0)
    {
        cout << path.append("intrinsics.xml").c_str() << " not found" << endl;
        return false;
    }

    if(distortion_coeffs == 0)
    {
        cout << path.append("distortion.xml").c_str() << " not found" << endl;
        return false;
    }

    return true;
}

void ProjectionModel::calculateProjection()
{
    if(intrinsic_matrix != 0 && distortion_coeffs != 0)
    {
        int corner_count = Chessboard::board_total;

        cvCvtColor(sourceImg, gray_image, CV_RGB2GRAY);

        int found = cvFindChessboardCorners(gray_image,
                                            board_sz,
                                            corners,
                                            &corner_count,
                                            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        if(!found)
        {
            return;
        }

        cvFindCornerSubPix(gray_image,
                           corners,
                           corner_count,
                           cvSize(11,11),
                           cvSize(-1,-1),
                           cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));

        objPts[0].x = 0;
        objPts[0].y = 0;
        objPts[1].x = Chessboard::board_w - 1;
        objPts[1].y = 0;
        objPts[2].x = 0;
        objPts[2].y = Chessboard::board_h - 1;
        objPts[3].x = Chessboard::board_w - 1;
        objPts[3].y = Chessboard::board_h - 1;
        imgPts[3] = corners[0];
        imgPts[2] = corners[Chessboard::board_w - 1];
        imgPts[1] = corners[(Chessboard::board_h - 1) * Chessboard::board_w];
        imgPts[0] = corners[(Chessboard::board_h - 1) * Chessboard::board_w + Chessboard::board_w - 1];

        cvGetPerspectiveTransform(objPts, imgPts, H);

        for(int i = 0; i < 4; ++i)
        {
            CV_MAT_ELEM(*image_points, CvPoint2D32f, i, 0) = imgPts[i];
            CV_MAT_ELEM(*object_points, CvPoint3D32f, i, 0) = cvPoint3D32f(objPts[i].x, objPts[i].y, 0);
        }

        cvFindExtrinsicCameraParams2(object_points,
                                     image_points,
                                     intrinsic_matrix,
                                     distortion_coeffs,
                                     rotation_vector,
                                     translation_vector);

        cvRodrigues2(rotation_vector, rotation_matrix);

        for(int f = 0; f < 3; f++)
        {
            for(int c = 0; c < 3; c++)
            {
                fullMatrix[c * 4 + f] = rotation_matrix->data.fl[f * 3 + c];   //transposed
            }
        }

        fullMatrix[3] = 0.0;
        fullMatrix[7] = 0.0;
        fullMatrix[11] = 0.0;
        fullMatrix[12] = translation_vector->data.fl[0];
        fullMatrix[13] = translation_vector->data.fl[1];
        fullMatrix[14] = translation_vector->data.fl[2];
        fullMatrix[15] = 1.0;
    }
}
