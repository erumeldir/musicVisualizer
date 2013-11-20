#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Matrix4.h"
#include "Vector3.h"

class Camera
{
  private:
	void updateCamera();	//updates our internal camera state based on e d and up
	void updateView();		//calculates the view matrix

  protected:
	Matrix4 C;		//the camera matrix
	Matrix4 V;		//the view matrix (inverse of the camera matrix)
	Vector3 e;		//the center of projection
	Vector3 d;		//position looking at
	Vector3 up;		//camera's up vector
	
  public:
	Camera();

	void setLocation(double, double, double );  //move our camera to (x,y,z)
	void setLocation(Vector3);					//move our camera to vector pos

	void lookAt(double, double, double); //set point camera looks at to (x,y,z)
	void lookAt(Vector3);				 //set point camera looks at to vector pos

	void setUpVector(Vector3);	//update the camera's roll (up vector)

	Matrix4& getCamMatrix();		//return the matrix object
	Matrix4& getViewMatrix();		//return the matrix in a double array w/ length 16
};

#endif