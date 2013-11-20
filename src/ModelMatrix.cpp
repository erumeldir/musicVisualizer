/*
 * ModelMatrix.cpp
 * Michael Taberner
 * 
 * Handles all standard transformations performed on models
 */

#include "ModelMatrix.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"

#include <iostream>
using namespace std;

ModelMatrix::ModelMatrix() 
{
	model.identity();
	scale.set(1,1,1);
	pos.set(0,0,0);
}

ModelMatrix::~ModelMatrix()
{

}

/******************************SCALE FUNCTIONS********************************/

/*
 * scale the object uniformly at the current position
 */
void ModelMatrix::globalScale(double s)
{
	Matrix4 scaleMatrix;
	scale.scale(s);
	scaleMatrix.scale(s,s,s);
	
	model.multiply(scaleMatrix);
}

/*
 * scale the object non-uniformly at the current position
 */
void ModelMatrix::globalScale(double sx, double sy, double sz)
{
	Matrix4 scaleMatrix;
	scale.set(scale.get(0)*sx,scale.get(1)*sy,scale.get(2)*sz);
	scaleMatrix.scale(sx,sy,sz);
		
	model.multiply(scaleMatrix);
}

/*
 * scale the object around a given point
 */
void ModelMatrix::scaleAround(Vector3 pt, double sx, double sy, double sz) 
{
	Matrix4 trans;
	Matrix4 scaleMatrix;

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//scale
	scale.set(scale.get(0)*sx,scale.get(1)*sy,scale.get(2)*sz);
	scaleMatrix.scale(sx,sy,sz);
	model.multiply(scaleMatrix);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
}

/*
 * scale the object around a given point
 */
void ModelMatrix::ScaleAround(Vector4 pt, double sx, double sy, double sz)
{
	Matrix4 trans;
	Matrix4 scaleMatrix;

	//translate to origin
	pt.scale(-1.0);
	trans.translate(Vector3(pt.get(0),pt.get(1),pt.get(2)));
	model.multiply(trans);
	
	//scale
	scale.set(scale.get(0)*sx,scale.get(1)*sy,scale.get(2)*sz);
	scaleMatrix.scale(sx,sy,sz);
	model.multiply(scaleMatrix);

	//translate back
	pt.scale(-1.0);
	trans.translate(Vector3(pt.get(0),pt.get(1),pt.get(2)));
	model.multiply(trans);
}

/*
 * uniform scale at the object's center
 */
void ModelMatrix::localScale(double s)
{
	Matrix4 trans;
	Matrix4 scaleMatrix;
	Vector3 pt;
	pt.setVector(pos);

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//scale
	scale.scale(s);
	scaleMatrix.scale(s,s,s);
	model.multiply(scaleMatrix);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);

}

/*
 * non-uniform scale at the object's center
 */
void ModelMatrix::localScale(double sx, double sy, double sz)
{
	Matrix4 trans;
	Matrix4 scaleMatrix;
	Vector3 pt;
	pt.setVector(pos);

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//scale
	scale.set(scale.get(0)*sx,scale.get(1)*sy,scale.get(2)*sz);
	scaleMatrix.scale(sx,sy,sz);
	model.multiply(scaleMatrix);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
}

/****************************ROTATION FUNCTIONS*******************************/

/*
 * Rotate the current matrix around the x-axis. Angles in radians
 */
void ModelMatrix::localRotateX(double angle)
{
	Matrix4 rot;
	rot.rotateX(angle);

	model.multiply(rot);
}

/*
 * Rotate the current matrix around the y-axis. Angles in radians
 */
void ModelMatrix::localRotateY(double angle)
{
	Matrix4 rot;
	rot.rotateY(angle);

	model.multiply(rot);
}

/*
 * Rotate the current matrix around the y-axis. Angles in radians
 */
void ModelMatrix::localRotateZ(double angle)
{
	Matrix4 rot;
	rot.rotateZ(angle);

	model.multiply(rot);
}

/*
 * Rotate the current matrix around a given axis. Angles in radians
 */
void ModelMatrix::localAxisRotate(double angle, Vector3 axis)
{
	Matrix4 rot;
	rot.rotate(angle,axis);

	model.multiply(rot);
}

/*
 * Rotate globally (at the origin) the current matrix around the x-axis. 
 * Angles in radians
 */
void ModelMatrix::globalRotateX(double angle)
{
	Matrix4 trans;
	Matrix4 rot;
	Vector3 pt;
	pt.setVector(pos);

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//rotate
	rot.rotateX(angle);
	model.multiply(rot);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
}

/*
 * Rotate globally (at the origin) the current matrix around the y-axis. 
 * Angles in radians
 */
void ModelMatrix::globalRotateY(double angle)
{
	Matrix4 trans;
	Matrix4 rot;
	Vector3 pt;
	pt.setVector(pos);

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//rotate
	rot.rotateY(angle);
	model.multiply(rot);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
}

/*
 * Rotate globally (at the origin) the current matrix around the z-axis. 
 * Angles in radians
 */
void ModelMatrix::globalRotateZ(double angle)
{
	Matrix4 trans;
	Matrix4 rot;
	Vector3 pt;
	pt.setVector(pos);

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//rotate
	rot.rotateZ(angle);
	model.multiply(rot);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
}

/*
 * Rotate globally (at the origin) the current matrix around given axis. 
 * Angles in radians
 */
void ModelMatrix::globalAxisRotate(double angle, Vector3 axis)
{
	Matrix4 trans;
	Matrix4 rot;
	Vector3 pt;
	pt.setVector(pos);

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//rotate
	rot.rotate(angle,axis);
	model.multiply(rot);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
}

/*
 * Rotate the current matrix at a point around the x-axis. 
 * Angles in radians
 */
void ModelMatrix::pointRotateX(double angle, Vector3 pt)
{
	Matrix4 trans;
	Matrix4 rot;

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//rotate
	rot.rotateX(angle);
	model.multiply(rot);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
}

/*
 * Rotate the current matrix at a point around the x-axis. 
 * Angles in radians
 */
void ModelMatrix::pointRotateY(double angle, Vector3 pt)
{
	Matrix4 trans;
	Matrix4 rot;

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//rotate
	rot.rotateY(angle);
	model.multiply(rot);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
}

/*
 * Rotate the current matrix at a point around the x-axis. 
 * Angles in radians
 */
void ModelMatrix::pointRotateZ(double angle, Vector3 pt)
{
	Matrix4 trans;
	Matrix4 rot;

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//rotate
	rot.rotateZ(angle);
	model.multiply(rot);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
}

/*
 * Rotate the current matrix at a point around the x-axis. 
 * Angles in radians
 */
void ModelMatrix::pointAxisRotate(double angle, Vector3 axis, Vector3 pt)
{
	Matrix4 trans;
	Matrix4 rot;

	//translate to origin
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
	
	//rotate
	rot.rotate(angle,axis);
	model.multiply(rot);

	//translate back
	pt.scale(-1.0);
	trans.translate(pt);
	model.multiply(trans);
}

/***************************TRANSLATION FUNCTIONS*****************************/

/*
 * Translate locally (affected by rotaiton) by given x,y,z
 */
void ModelMatrix::localTranslate(double dx, double dy, double dz)
{
	Matrix4 trans;
	trans.translate(dx,dy,dz);
	
	model.multiply(trans);
	pos.set(model.get(3,0),model.get(3,1),model.get(3,2));
}

/*
 * Translate locally (affected by rotaiton) by given vector
 */
void ModelMatrix::localTranslate(Vector3 t)
{
	Matrix4 trans;
	trans.translate(t);
	
	model.multiply(trans);
	pos.set(model.get(3,0),model.get(3,1),model.get(3,2));
}

/*
 * Translate globally (ignores rotation) by given x,y,z
 */
void ModelMatrix::globalTranslate(double dx, double dy, double dz)
{
	Matrix4 trans;
	trans.translate(dx,dy,dz);

	trans.multiply(model);
	model.set(trans);
	pos.set(model.get(3,0),model.get(3,1),model.get(3,2));

	model.print();
	pos.print();

	cout << "(0,3): " << model.get(0,3) << endl;
	cout << "(3,0): " << model.get(3,0) << endl;
}

/*
 * Translate globally (ignores rotation) by given vector
 */
void ModelMatrix::globalTranslate(Vector3 t)
{
	Matrix4 trans;
	trans.translate(t);

	trans.multiply(model);
	model.set(trans);
	pos.set(model.get(3,0),model.get(3,1),model.get(3,2));
}

/***********************************UTILS*************************************/

/*
 * re-init the model matrix
 */
void ModelMatrix::reset()
{
	model.identity();
	scale.set(1,1,1);
	pos.set(0,0,0);
}

/*
 * return the matrix in a double array
 */
double* ModelMatrix::getGLModelview()
{
	return model.getPointer();
}

/*
 * combine the model matrix and given camera matrix and return in double array
 */
double* ModelMatrix::getGLModelview(Matrix4 invCamera)
{
	invCamera.multiply(model);
	return invCamera.getPointer();
}

/*********************************GET AND SET*********************************/

/*
 * returns the current rotation component of the matrix
 */
Matrix4 ModelMatrix::getRotation()
{
	Matrix4 result;
	result.set(model);
	result.set(0,0,model.get(0,0)/scale.get(0));
	result.set(1,1,model.get(1,1)/scale.get(1));
	result.set(2,2,model.get(2,2)/scale.get(2));
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);

	return result;
}