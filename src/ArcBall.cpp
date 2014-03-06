/** KempoApi: The Turloc Toolkit *****************************/
/** *    *                                                  **/
/** **  **  Filename: ArcBall.cpp                           **/
/**   **    Version:  Common                                **/
/**   **                                                    **/
/**                                                         **/
/**  Arcball class for mouse manipulation.                  **/
/**                                                         **/
/**                                                         **/
/**                                                         **/
/**                                                         **/
/**                              (C) 1999-2003 Tatewake.com **/
/**   History:                                              **/
/**   08/17/2003 - (TJG) - Creation                         **/
/**   09/23/2003 - (TJG) - Bug fix and optimization         **/
/**   09/25/2003 - (TJG) - Version for NeHe Basecode users  **/
/**                                                         **/
/*************************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

#include "math.h"
#include "stdlib.h"
#include "string.h"

#include "ArcBall.h"

ArcBall::ArcBall(GLfloat NewWidth, GLfloat NewHeight)
{
    //Clear initial values
    this->StVec.s.X = this->StVec.s.Y = this->StVec.s.Z =

    this->EnVec.s.X = this->EnVec.s.Y = this->EnVec.s.Z = 0.0f;

    //Set initial bounds
    this->setBounds(NewWidth, NewHeight);

    GLfloat four[16]   = {  1.0f,  0.0f,  0.0f,  0.0f,
                                 0.0f,  1.0f,  0.0f,  0.0f,
                                 0.0f,  0.0f,  1.0f,  0.0f,
                                 0.0f,  0.0f,  0.0f,  1.0f };

    GLfloat three[12]     = {  1.0f,  0.0f,  0.0f,
                                 0.0f,  1.0f,  0.0f,
                                 0.0f,  0.0f,  1.0f };

    memcpy(&Transform.M[0], &four[0], sizeof(GLfloat) * 16);
    memcpy(&LastRot.M[0], &three[0], sizeof(GLfloat) * 9);
    memcpy(&ThisRot.M[0], &three[0], sizeof(GLfloat) * 9);

    ThisQuat = new Quat4fT;
}

ArcBall::~ArcBall()
{
    delete ThisQuat;
}

void ArcBall::imageToSphereCoords(const Point2fT* NewPt, Vector3fT* NewVec) const
{
    Point2fT TempPt;
    GLfloat length;

    //Copy paramter into temp point
    TempPt = *NewPt;

    //Adjust point coords and scale down to range of [-1 ... 1]
    TempPt.s.X = (TempPt.s.X * this->AdjustWidth) - 1.0f;
    TempPt.s.Y = 1.0f - (TempPt.s.Y * this->AdjustHeight);

    //Compute the square of the length of the vector to the point from the center
    length = (TempPt.s.X * TempPt.s.X) + (TempPt.s.Y * TempPt.s.Y);

    //If the point is mapped outside of the sphere... (length > radius squared)
    if(length > 1.0f)
    {
        GLfloat norm;

        //Compute a normalizing factor (radius / sqrt(length))
        norm = 1.0f / FuncSqrt(length);

        //Return the "normalized" vector, a point on the sphere
        NewVec->s.X = TempPt.s.X * norm;
        NewVec->s.Y = TempPt.s.Y * norm;
        NewVec->s.Z = 0.0f;
    }
    else //Else it's on the inside
    {
        //Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
        NewVec->s.X = TempPt.s.X;
        NewVec->s.Y = TempPt.s.Y;
        NewVec->s.Z = FuncSqrt(1.0f - length);
    }
}

void ArcBall::click(const int inX, const int inY)
{
    LastRot = ThisRot;
    Point2fT NewPt;
    NewPt.s.X = inX;
    NewPt.s.Y = inY;
    //Map the point to the sphere
    this->imageToSphereCoords(&NewPt, &this->StVec);
}

GLfloat * ArcBall::getTransform()
{
    return &Transform.M[0];
}

//Mouse drag, calculate rotation
void ArcBall::drag(const int inX, const int inY)
{
    Point2fT NewPt;
    NewPt.s.X = inX;
    NewPt.s.Y = inY;
    //Map the point to the sphere
    this->imageToSphereCoords(&NewPt, &this->EnVec);

    //Return the quaternion equivalent to the rotation
    if(ThisQuat)
    {
        Vector3fT Perp;

        //Compute the vector perpendicular to the begin and end vectors
        Vector3fCross(&Perp, &this->StVec, &this->EnVec);

        //Compute the length of the perpendicular vector
        if(Vector3fLength(&Perp) > Epsilon) //if its non-zero
        {
            //We're ok, so return the perpendicular vector as the transform after all
            ThisQuat->s.X = Perp.s.X;
            ThisQuat->s.Y = Perp.s.Y;
            ThisQuat->s.Z = Perp.s.Z;
            //In the quaternion values, w is cosine (theta / 2), where theta is rotation angle
            ThisQuat->s.W = Vector3fDot(&this->StVec, &this->EnVec);
        }
        else //if its zero
        {
            //The begin and end vectors coincide, so return an identity transform
            ThisQuat->s.X = ThisQuat->s.Y = ThisQuat->s.Z = ThisQuat->s.W = 0.0f;
        }
    }

    Matrix3fSetRotationFromQuat4f(&ThisRot, ThisQuat);     // Convert Quaternion Into Matrix3fT
    Matrix3fMulMatrix3f(&ThisRot, &LastRot);                // Accumulate Last Rotation Into This One
    Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);  // Set Our Final Transform's Rotation From This One
}

