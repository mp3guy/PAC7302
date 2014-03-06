#ifndef TESCOCAM_H
#define TESCOCAM_H

#include <QtGui/QWidget>
#include <qimage.h>
#include <qtimer.h>
#include <iostream>
#include "../build/ui_tescocam.h"
#include "V4LCam.h"
#include "CameraCalibration.h"
#include "ImageCorrector.h"
#include "ImageProjection.h"
#include "ProjectionModel.h"

using namespace std;

class TescoCam : public QWidget
{
    Q_OBJECT

    public:
        TescoCam(QWidget *parent = 0);
        ~TescoCam();

    private:
        Ui::TescoCamClass ui;
        QImage * imageData;
        IplImage * chessboardImage;

        V4LCam * myCam;
        CameraCalibration * myCalib;
        ImageCorrector * myImageCorrector;
        ImageProjection * myImageProjection;
        ProjectionModel * myProjectModel;

        int timerId;

        enum State
        {
            VIEWING,
            CALIBRATING
        };

        State currentState;

        void setupGui();
        void timerEvent(QTimerEvent * event);
        void setupControlSliders();
        void populateNewSettings();

    private slots:
        void calibrateClick()
        {
            if(currentState != CALIBRATING)
            {
                ui.calibrateButton->setText("Cancel Calibration");
                currentState = CALIBRATING;
            }
            else
            {
                ui.calibrateButton->setText("Calibrate");
                currentState = VIEWING;

                if(myCalib != 0)
                {
                    delete myCalib;
                    myCalib = 0;
                }
            }
        }
};

#endif // TESCOCAM_H
