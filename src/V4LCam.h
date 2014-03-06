/*
 * V4LCam.h
 *
 *  Created on: 27 Jul 2011
 *      Author: thomas, and lots of borrowed code from the 2010
 *      b-human code release (Colin Graf & Thomas Röfer)
 */

#ifndef V4LCAM_H_
#define V4LCAM_H_

#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <iostream>
#include <utility>
#include <string>
#include <libv4lconvert.h>

#include "pac7302.h"

using namespace std;

class V4LCam
{
    public:
        V4LCam(unsigned char * imgBuffer);
        virtual ~V4LCam();

        bool captureNew();

        bool getImage();

        unsigned getTimeStamp() const;

        class Pac7302Settings
        {
            public:
                pair<v4l2_queryctrl, v4l2_control> brightness;
                pair<v4l2_queryctrl, v4l2_control> contrast;
                pair<v4l2_queryctrl, v4l2_control> colors;
                pair<v4l2_queryctrl, v4l2_control> whiteBalance;
                pair<v4l2_queryctrl, v4l2_control> redBalance;
                pair<v4l2_queryctrl, v4l2_control> blueBalance;
                pair<v4l2_queryctrl, v4l2_control> autogain;
                pair<v4l2_queryctrl, v4l2_control> gain;
                pair<v4l2_queryctrl, v4l2_control> exposure;

                Pac7302Settings()
                {
                    brightness.first.id = V4L2_CID_BRIGHTNESS;
                    contrast.first.id = V4L2_CID_CONTRAST;
                    colors.first.id = V4L2_CID_SATURATION;
                    whiteBalance.first.id = V4L2_CID_WHITE_BALANCE_TEMPERATURE;
                    redBalance.first.id = V4L2_CID_RED_BALANCE;
                    blueBalance.first.id = V4L2_CID_BLUE_BALANCE;
                    autogain.first.id = V4L2_CID_AUTOGAIN;
                    gain.first.id = V4L2_CID_GAIN;
                    exposure.first.id = V4L2_CID_EXPOSURE;

                    brightness.second.id = V4L2_CID_BRIGHTNESS;
                    contrast.second.id = V4L2_CID_CONTRAST;
                    colors.second.id = V4L2_CID_SATURATION;
                    whiteBalance.second.id = V4L2_CID_WHITE_BALANCE_TEMPERATURE;
                    redBalance.second.id = V4L2_CID_RED_BALANCE;
                    blueBalance.second.id = V4L2_CID_BLUE_BALANCE;
                    autogain.second.id = V4L2_CID_AUTOGAIN;
                    gain.second.id = V4L2_CID_GAIN;
                    exposure.second.id = V4L2_CID_EXPOSURE;
                }
        };

        const Pac7302Settings & getCurrentSettings()
        {
            return currentSettings;
        }

        void applyNewSettings(Pac7302Settings & newSettings)
        {
            currentSettings = newSettings;
            writeCameraSettings();
        }

    private:
        void getControlSetting(pair<v4l2_queryctrl, v4l2_control> & control);
        void setControlSetting(pair<v4l2_queryctrl, v4l2_control> & control);

        void initOpenVideoDevice();
        void initRequestAndMapBuffers();
        void initQueueAllBuffers();
        void startCapturing();
        void writeCameraSettings();

        Pac7302Settings currentSettings;

        struct v4lconvert_data * v4lconvert_data;
        struct v4l2_format src_fmt;
        struct v4l2_format fmt;

        static V4LCam * theInstance;

        static const int frameBufferCount = 2;

        unsigned char * imageBuffer;

        int fd;
        void* mem[frameBufferCount];
        int memLength[frameBufferCount];
        struct v4l2_buffer* buf;
        struct v4l2_buffer* currentBuf;
        unsigned timeStamp;
};

#endif /* V4LCAM_H_ */
