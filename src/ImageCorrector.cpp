/*
 * ImageCorrector.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: thomas
 */

#include "ImageCorrector.h"

ImageCorrector::ImageCorrector(unsigned char * imageData, string path)
{
    this->path = path;

    mapx = cvCreateImage(cvSize(pac7302::WIDTH, pac7302::HEIGHT), IPL_DEPTH_32F, 1);
    mapy = cvCreateImage(cvSize(pac7302::WIDTH, pac7302::HEIGHT), IPL_DEPTH_32F, 1);

    sourceImg = cvCreateImageHeader(cvSize(pac7302::WIDTH, pac7302::HEIGHT), IPL_DEPTH_8U, 3);
    sourceImg->imageData = (char *)imageData;

    intrinsic_matrix = 0;
    distortion_coeffs = 0;
}

ImageCorrector::~ImageCorrector()
{
    cvReleaseImage(&mapx);
    cvReleaseImage(&mapy);
    cvReleaseImageHeader(&sourceImg);

    if(intrinsic_matrix != 0)
    {
        cvReleaseMat(&intrinsic_matrix);
    }

    if(distortion_coeffs != 0)
    {
        cvReleaseMat(&distortion_coeffs);
    }
}

bool ImageCorrector::loadData()
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

    cvInitUndistortMap(intrinsic_matrix, distortion_coeffs, mapx, mapy);

    return true;
}

void ImageCorrector::correct()
{
    if(intrinsic_matrix != 0 && distortion_coeffs != 0)
    {
        tempImg = cvCloneImage(sourceImg);
        cvRemap(tempImg, sourceImg, mapx, mapy);
        cvReleaseImage(&tempImg);
    }
}
