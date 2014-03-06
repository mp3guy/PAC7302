/*
 * V4LCam.cpp
 *
 *  Created on: 27 Jul 2011
 *      Author: thomas, and lots of borrowed code from the 2010
 *      b-human code release (Colin Graf & Thomas Röfer)
 */

#include "V4LCam.h"

V4LCam * V4LCam::theInstance = 0;

V4LCam::V4LCam(unsigned char * imgBuffer)
 : imageBuffer(imgBuffer),
   currentBuf(0),
   timeStamp(0)
{
    theInstance = this;

    initOpenVideoDevice();
    initRequestAndMapBuffers();
    initQueueAllBuffers();

    src_fmt.fmt.pix.pixelformat = v4l2_fourcc('P', 'J', 'P', 'G');
    src_fmt.fmt.pix.width = pac7302::WIDTH;
    src_fmt.fmt.pix.height = pac7302::HEIGHT;
    src_fmt.fmt.pix.bytesperline = pac7302::WIDTH;
    src_fmt.fmt.pix.sizeimage = pac7302::WIDTH * pac7302::HEIGHT;
    memcpy(&fmt, &src_fmt, sizeof fmt);
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.sizeimage = pac7302::WIDTH * pac7302::HEIGHT * 3;

    string envString = "LIBV4LCONTROL_CONTROLS=15";

    putenv((char *)envString.c_str());

    v4lconvert_data = v4lconvert_create(fd);

    getControlSetting(currentSettings.autogain);
    getControlSetting(currentSettings.blueBalance);
    getControlSetting(currentSettings.brightness);
    getControlSetting(currentSettings.colors);
    getControlSetting(currentSettings.contrast);
    getControlSetting(currentSettings.exposure);
    getControlSetting(currentSettings.gain);
    getControlSetting(currentSettings.redBalance);
    getControlSetting(currentSettings.whiteBalance);

    startCapturing();
}

V4LCam::~V4LCam()
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd, VIDIOC_STREAMOFF, &type);

    for(int i = 0; i < frameBufferCount; ++i)
        munmap(mem[i], memLength[i]);

    close(fd);
    free(buf);

    v4lconvert_destroy(v4lconvert_data);

    theInstance = 0;
}

bool V4LCam::captureNew()
{
    if(currentBuf)
    {
        if(ioctl(fd, VIDIOC_QBUF, currentBuf) == -1)
        {
            return false;
        }
    }

    if(ioctl(fd, VIDIOC_DQBUF, buf) == -1)
    {
        return false;
    }

    struct timeval tv;
    gettimeofday(&tv, 0);
    unsigned int time = (unsigned int) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
    static unsigned base = 0;
    if(!base)
        base = time - 10000;
    timeStamp = time - base;

    currentBuf = buf;

    return true;
}

bool V4LCam::getImage()
{
    if(currentBuf)
    {
        int val = v4lconvert_convert(v4lconvert_data,
                          &src_fmt,
                          &fmt,
                          static_cast<unsigned char*>(mem[currentBuf->index]), currentBuf->bytesused,
                          &imageBuffer[0], fmt.fmt.pix.sizeimage);

        if(val == -1)
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

unsigned int V4LCam::getTimeStamp() const
{
    return timeStamp;
}

void V4LCam::writeCameraSettings()
{
    setControlSetting(currentSettings.autogain);
    setControlSetting(currentSettings.blueBalance);
    setControlSetting(currentSettings.brightness);
    setControlSetting(currentSettings.colors);
    setControlSetting(currentSettings.contrast);
    setControlSetting(currentSettings.exposure);
    setControlSetting(currentSettings.gain);
    setControlSetting(currentSettings.redBalance);
    setControlSetting(currentSettings.whiteBalance);
}

void V4LCam::startCapturing()
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd, VIDIOC_STREAMON, &type);
}

void V4LCam::getControlSetting(pair<v4l2_queryctrl, v4l2_control> & control)
{
    ioctl(fd, VIDIOC_QUERYCTRL, &control.first);
    ioctl(fd, VIDIOC_G_CTRL, &control.second);
}

void V4LCam::setControlSetting(pair<v4l2_queryctrl, v4l2_control> & control)
{
    if(control.second.value < control.first.minimum)
    {
        control.second.value = control.first.minimum;
    }

    if(control.second.value > control.first.maximum)
    {
        control.second.value = control.first.maximum;
    }

    if(control.second.value < 0)
    {
        control.second.value = control.first.default_value;
    }

    ioctl(fd, VIDIOC_S_CTRL, &control.second);
}

void V4LCam::initOpenVideoDevice()
{
    fd = open("/dev/video0", O_RDWR);
}

void V4LCam::initRequestAndMapBuffers()
{
    struct v4l2_requestbuffers rb;
    memset(&rb, 0, sizeof(struct v4l2_requestbuffers));
    rb.count = frameBufferCount;
    rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    rb.memory = V4L2_MEMORY_MMAP;
    ioctl(fd, VIDIOC_REQBUFS, &rb);

    buf = static_cast<struct v4l2_buffer*>(calloc(1, sizeof   (struct v4l2_buffer)));

    for(int i = 0; i < frameBufferCount; ++i)
    {
        buf->index = i;
        buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf->memory = V4L2_MEMORY_MMAP;
        ioctl(fd, VIDIOC_QUERYBUF, buf);
        memLength[i] = buf->length;
        mem[i] = mmap(0, buf->length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf->m.offset);
    }
}

void V4LCam::initQueueAllBuffers()
{
    for(int i = 0; i < frameBufferCount; ++i)
    {
        buf->index = i;
        buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf->memory = V4L2_MEMORY_MMAP;
        ioctl(fd, VIDIOC_QBUF, buf);
    }
}
