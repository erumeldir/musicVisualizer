/*
 * Contains all classes needed to draw the opengl 3d primitives
 * All classes inherit from Geode
 */
#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Node.h"
#include "Vector3.h"
#include "Frustum.h"
#include <vector>

/*
 * Sphere class. Contains all methods/vars needed to draw a sphere
 */
class Sphere : public Geode
{
protected:
	double radius;
	double slices;
	double stacks;

public:
	Sphere(double,double,double);
	~Sphere();

	void draw(Matrix4,Frustum,bool);
	void computeBoundingSphere(Matrix4);
};

class Cube : public Geode
{
protected:
	double size;

public:
	Cube(double);
	~Cube();

	void draw(Matrix4,Frustum,bool);
};

class RectanglePrism : public Geode
{
protected:
	double width;
	double length;
	double height;

	Matrix4 rectangleScale;

public:
	RectanglePrism(double,double,double);
	~RectanglePrism();

	void draw(Matrix4,Frustum,bool);
	void computeBoundingSphere(Matrix4);
};

/*
 * Bezier curve  class
 */
class BezierCurve : public Geode
{
protected:

	static const int SUBDIVISIONS = 20;

	//Control point class
	class ControlPoint 
	{
	public:
		//enum for the type of control point
		typedef enum CPointType 
		{ 
			INTERPOLATING, 
			APPROXIMATING 
		} CPointType;

		ControlPoint(Vector4, CPointType);
		~ControlPoint();

		Vector4 getPoint();				//returnt the position
		void setPoint(Vector4);			//sets the point at the new position

		CPointType getPointType();		//return the point type
		void setPointType(CPointType);	//set the points type

	private:
		Vector4 pos;
		CPointType type;	
	};

	typedef ControlPoint::CPointType CPointType;

	vector<ControlPoint*> controlPoints;	//list of all control points

public:
	//Bezier matrix
	static Matrix4 B;
	static Matrix4 Bnorm;

	//TODO add constructor which allows a curve of any number of points
	BezierCurve();		//default constructor which creates 2 attached curves
	~BezierCurve();

	virtual void draw(Matrix4,Frustum,bool);
	virtual void computeBoundingSphere(Matrix4);

	int getPointIndex(Vector4,double);	//returns the index of a control point
	void movePoint(Vector4,int);		//move the point at given index
};

/*
 * Surface of Revolution class
 */
class RevolutionSurface : public BezierCurve
{
private:
	ModelMatrix rotation;	//rotation matrix
	double rotDelta;		//angle of rotation
	bool showTexture;		//toggle texture on and off
	bool curveDisplayed;	//toggle curve on and off

public:
	RevolutionSurface();		//create a surface of revolution
	RevolutionSurface(double);	//create a surface of revolution with given rot angle
	~RevolutionSurface();

	void draw(Matrix4,Frustum,bool);
	void computeBoundingSphere(Matrix4);
	void enableTexture();
	void disableTexture();
	void showCurve();
	void hideCurve();
};

#endif