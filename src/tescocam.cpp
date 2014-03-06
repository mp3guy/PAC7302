#include "tescocam.h"

TescoCam::TescoCam(QWidget * parent)
 : QWidget(parent)
{
	ui.setupUi(this);

	imageData = new QImage(pac7302::WIDTH, pac7302::HEIGHT, QImage::Format_RGB888);

	chessboardImage = cvLoadImage("checkerboard.png");

	myCam = new V4LCam(imageData->bits());

	myImageCorrector = new ImageCorrector(imageData->bits(), "");

    myProjectModel = new ProjectionModel(imageData->bits(), "");

	myImageProjection = new ImageProjection(chessboardImage, myProjectModel->getMatrix());

	myCalib = 0;

	currentState = VIEWING;

	setupGui();

    timerId = startTimer(1);
}

TescoCam::~TescoCam()
{
    delete imageData;
    delete myCam;
    delete myImageCorrector;
    delete myImageProjection;
    delete myProjectModel;

    cvReleaseImage(&chessboardImage);

    if(myCalib != 0)
    {
        delete myCalib;
    }
}

void TescoCam::setupGui()
{
    setupControlSliders();

    myImageProjection->setFixedSize(ImageProjection::GL_WIN_WIDTH, ImageProjection::GL_WIN_HEIGHT);

    ui.glHorizontalLayout->addWidget(myImageProjection);

    ui.correctedCheckBox->setEnabled(myImageCorrector->loadData());

    myProjectModel->loadData();

    connect(ui.calibrateButton, SIGNAL(clicked(bool)), this, SLOT(calibrateClick()));
}

void TescoCam::timerEvent(QTimerEvent * event)
{
    if(event->type() == 1 && myCam->captureNew() && myCam->getImage())
    {
        populateNewSettings();

        if(currentState == VIEWING)
        {
            if(ui.correctedCheckBox->isChecked())
            {
                myImageCorrector->correct();
            }
        }
        else if(currentState == CALIBRATING)
        {
            if(myCalib == 0)
            {
                myCalib = new CameraCalibration(imageData->bits(), 10, "");
            }

            if(!myCalib->isDone())
            {
                myCalib->calibrationStep();
            }
            else
            {
                ui.correctedCheckBox->setEnabled(myImageCorrector->loadData());

                if(ui.correctedCheckBox->isEnabled())
                {
                    ui.correctedCheckBox->setChecked(true);
                }

                calibrateClick();
            }

            if(ui.correctedCheckBox->isChecked())
            {
                myImageCorrector->correct();
            }
        }

        myProjectModel->calculateProjection();
        myImageProjection->updateGL();
        ui.imageHolder->setPixmap(QPixmap::fromImage(*imageData));
    }
}

void TescoCam::populateNewSettings()
{
    V4LCam::Pac7302Settings newSettings = myCam->getCurrentSettings();

    newSettings.brightness.second.value = ui.brightnessSlider->value();
    newSettings.contrast.second.value = ui.contrastSlider->value();
    newSettings.colors.second.value = ui.saturationSlider->value();
    newSettings.whiteBalance.second.value = ui.whiteBalSlider->value();
    newSettings.redBalance.second.value = ui.redBalSlider->value();
    newSettings.blueBalance.second.value = ui.blueBalSlider->value();
    newSettings.gain.second.value = ui.gainSlider->value();
    newSettings.autogain.second.value = ui.autoGainSlider->value();
    newSettings.exposure.second.value = ui.exposureSlider->value();

    myCam->applyNewSettings(newSettings);
}

void TescoCam::setupControlSliders()
{
    const V4LCam::Pac7302Settings & currentSettings = myCam->getCurrentSettings();

    ui.brightnessSlider->setMaximum(currentSettings.brightness.first.maximum);
    ui.contrastSlider->setMaximum(currentSettings.contrast.first.maximum);
    ui.saturationSlider->setMaximum(currentSettings.colors.first.maximum);
    ui.whiteBalSlider->setMaximum(currentSettings.whiteBalance.first.maximum);
    ui.redBalSlider->setMaximum(currentSettings.redBalance.first.maximum);
    ui.blueBalSlider->setMaximum(currentSettings.blueBalance.first.maximum);
    ui.gainSlider->setMaximum(currentSettings.gain.first.maximum);
    ui.autoGainSlider->setMaximum(currentSettings.autogain.first.maximum);
    ui.exposureSlider->setMaximum(currentSettings.exposure.first.maximum);

    ui.brightnessSlider->setMinimum(currentSettings.brightness.first.minimum);
    ui.contrastSlider->setMinimum(currentSettings.contrast.first.minimum);
    ui.saturationSlider->setMinimum(currentSettings.colors.first.minimum);
    ui.whiteBalSlider->setMinimum(currentSettings.whiteBalance.first.minimum);
    ui.redBalSlider->setMinimum(currentSettings.redBalance.first.minimum);
    ui.blueBalSlider->setMinimum(currentSettings.blueBalance.first.minimum);
    ui.gainSlider->setMinimum(currentSettings.gain.first.minimum);
    ui.autoGainSlider->setMinimum(currentSettings.autogain.first.minimum);
    ui.exposureSlider->setMinimum(currentSettings.exposure.first.minimum);

    connect(ui.brightnessSlider, SIGNAL(valueChanged(int)), ui.brightnessVal, SLOT(setNum(int)));
    connect(ui.contrastSlider, SIGNAL(valueChanged(int)), ui.contrastVal, SLOT(setNum(int)));
    connect(ui.saturationSlider, SIGNAL(valueChanged(int)), ui.saturationVal, SLOT(setNum(int)));
    connect(ui.whiteBalSlider, SIGNAL(valueChanged(int)), ui.whiteBalVal, SLOT(setNum(int)));
    connect(ui.redBalSlider, SIGNAL(valueChanged(int)), ui.redBalVal, SLOT(setNum(int)));
    connect(ui.blueBalSlider, SIGNAL(valueChanged(int)), ui.blueBalVal, SLOT(setNum(int)));
    connect(ui.gainSlider, SIGNAL(valueChanged(int)), ui.gainVal, SLOT(setNum(int)));
    connect(ui.autoGainSlider, SIGNAL(valueChanged(int)), ui.autoGainVal, SLOT(setNum(int)));
    connect(ui.exposureSlider, SIGNAL(valueChanged(int)), ui.exposureVal, SLOT(setNum(int)));

    ui.brightnessSlider->setValue(currentSettings.brightness.second.value);
    ui.contrastSlider->setValue(currentSettings.contrast.second.value);
    ui.saturationSlider->setValue(currentSettings.colors.second.value);
    ui.whiteBalSlider->setValue(currentSettings.whiteBalance.second.value);
    ui.redBalSlider->setValue(currentSettings.redBalance.second.value);
    ui.blueBalSlider->setValue(currentSettings.blueBalance.second.value);
    ui.gainSlider->setValue(currentSettings.gain.second.value);
    ui.autoGainSlider->setValue(currentSettings.autogain.second.value);
    ui.exposureSlider->setValue(currentSettings.exposure.second.value);
}
