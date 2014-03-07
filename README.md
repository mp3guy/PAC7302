PAC7302
=======

Small application to configure/calibrate the PAC7302 USB camera (Technika H16WC-01 from Tesco).

Performs intrinsic and distortion parameter calibration. Also rectifies image and shows scaleless 3D pose of camera in OpenGL view. 

Requires OpenCV, Qt4, GLEW, GLUT and LibV4LConvert. 

Uses FindLibV4LConvert.cmake from [https://github.com/dos1/kamerka/blob/master/modules/FindLibV4LConvert.cmake](https://github.com/dos1/kamerka/blob/master/modules/FindLibV4LConvert.cmake).

Uses some code from the B-Human code release (http://www.b-human.de/). 

Worst camera ever. The settings don't seem to work, only white balance (which is actually auto!). Feel free to fix!

<p align="center">
  <img src="http://mp3guy.github.io/img/PAC7302.png" alt="PAC7302"/>
</p>

