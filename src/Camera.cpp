#include <math.h>
#include "Camera.h"

//constructor
Camera::Camera() : C(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
				   V(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
				   e(0,0,0),
				   d(0,0,0),
				   up(0,1,0)
{
	C.identity();
	V.identity();
}

/*
 * Set the camera location to (x,y,z)
 */
void Camera::setLocation(double x, double y, double z)
{
	e.set(x,y,z);
	updateCamera();
}

/*
 * Set the camera location to vector pos
 */
void Camera::setLocation(Vector3 pos)
{
	d = pos;
	updateCamera();
}

/*
 * Set the point the camera looks at
 */
void Camera::lookAt(double x, double y, double z)
{
	d.set(x,y,z);
	updateCamera();
}

/*
 * Set the point the camera looks at
 */
void Camera::lookAt(Vector3 pos)
{
	d = pos;
	updateCamera();
}

/*
 * Set the point the camera looks at
 */
void Camera::setUpVector(Vector3 newUp)
{
	up = newUp;
	updateCamera();
}

/*
 * compute the camera matrix
 */
void Camera::updateCamera()
{
	Vector3 u(0,0,0);
	Vector3 v(0,0,0);
	Vector3 w(0,0,0);
	
	//calculate the z projection
	w = e - d;
	w.normalize();

	//calculate the x projection
	u = up.cross(w);
	u.normalize();

	//calculate the y projection
	v = w.cross(u);

	//set the matrix
	C.identity();
	for(int i=0;i<3;i++)
	{
		C.set(0,i,u[i]);
	}
	
	for(int i=0;i<3;i++)
	{
		C.set(1,i,v[i]);
	}

	for(int i=0;i<3;i++)
	{
		C.set(2,i,w[i]);
	}

	for(int i=0;i<3;i++)
	{
		C.set(3,i,e[i]);
	}

	updateView();
}

/*
 *
 */
void Camera::updateView()
{
	Matrix4 translation;
	Matrix4 rotation(C);

	translation.translate(C.get(3,0)*-1,C.get(3,1)*-1,C.get(3,2)*-1);
	for(int i=0;i<3;i++)
		rotation.set(3,i,0);
	rotation.transpose();

	rotation*translation;
	V = rotation;
}

/*
 * return camera matrix
 */
Matrix4& Camera::getCamMatrix()
{
	return C;
}

/*
 * return pointer to matrix array (GL format specifies Column order, single
 * dimensional double array)
 */
Matrix4& Camera::getViewMatrix()
{
	return V;
}