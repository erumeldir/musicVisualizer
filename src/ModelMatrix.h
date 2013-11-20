#ifndef __MODEL_MATRIX_H__
#define __MODEL_MATRIX_H__

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"

class ModelMatrix
{
protected:
	Matrix4 model;
	Vector3 scale;
	Vector3 pos;


public:
	ModelMatrix();
	~ModelMatrix();

	//scale functions
	void globalScale(double);						//scale at the current objects location
	void globalScale(double,double,double);			//non-uniform scale at current object's location
	void scaleAround(Vector3,double,double,double); //scale around point
	void ScaleAround(Vector4,double,double,double);	//scale around point
	void localScale(double);						//scale relative to object's origin (center of object)
	void localScale(double,double,double);			//non-uniform scale relative to object's origin

	//rotation functions
	void localRotateX(double);						//rotate around the X axis
	void localRotateY(double);						//rotate around the Y axis
	void localRotateZ(double);						//rotate around the Z axis
	void localAxisRotate(double, Vector3);			//rotate around an arbitrary axis

	void globalRotateX(double);						//rotate globally around the X axis
	void globalRotateY(double);						//rotate globally around the Y axis
	void globalRotateZ(double);						//rotate globally around the Z axis
	void globalAxisRotate(double, Vector3);			//rotate globally around an arbitrary axis

	void pointRotateX(double, Vector3);				//rotate the X around a particular point
	void pointRotateY(double, Vector3);				//rotate the X around a particular point
	void pointRotateZ(double, Vector3);				//rotate the X around a particular point
	void pointAxisRotate(double, Vector3, Vector3); //rotate around a particular point and axis

	//translation functions
	void localTranslate(double,double,double);		//translate locally by given x,y,z
	void localTranslate(Vector3);					//translate locally by given vector
	void globalTranslate(double,double,double);		//translate globally by given x,y,z
	void globalTranslate(Vector3);					//translate globally by given vector
	
	//misc
	void reset();	//set to identity
	
	//GL MODELVIEW functions
	double* getGLModelview();
	double* getGLModelview(Matrix4 invCamera);

	//get functions
	Matrix4 getRotation();
	Matrix4 getMatrix() { return model; }
	Vector3 getScale()  { return scale; }
	Vector3 getPos()    { return pos; }
	
};

#endif