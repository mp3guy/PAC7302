/*
 * ImageCorrector.h
 *
 *  Created on: 1 Nov 2011
 *      Author: thomas
 */

#ifndef IMAGECORRECTOR_H_
#define IMAGECORRECTOR_H_

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "pac7302.h"

using namespace std;

class ImageCorrector
{
    public:
        ImageCorrector(unsigned char * imageData, string path);
        virtual ~ImageCorrector();

        bool loadData();
        void correct();

    private:
        IplImage * mapx;
        IplImage * mapy;
        IplImage * sourceImg;
        IplImage * tempImg;

        string path;

        CvMat * intrinsic_matrix;
        CvMat * distortion_coeffs;
};

#endif /* IMAGECORRECTOR_H_ */
