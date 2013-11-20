#define _USE_MATH_DEFINES

#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include "Geometry.h"
#include <iostream>
#include <float.h>

/********************************SPHERE CLASS*********************************/

/*
 * Constructor. create a sphere with given radius, slices and stacks
 */
Sphere::Sphere(double radius, double slices, double stacks) : radius(radius),
															  slices(slices),
															  stacks(stacks)
{
	shaderEnabled = false;
}

/*
 * Destructor. create a sphere with given radius, slices and stacks
 */
Sphere::~Sphere()
{
	
}

/*
 * Draw the sphere
 */
void Sphere::draw(Matrix4 C, Frustum F, bool checkCulling)
{
	Frustum::SphereCollision collState;
	if(checkCulling)
		collState = checkFrustumCulling(F);
	else
		collState = Frustum::INSIDE;

	//don't draw if sphere is outside
	if(collState != Frustum::OUTSIDE)
	{
		glLoadMatrixd(C.getPointer());
		glutSolidSphere(radius,slices,stacks);
	}
}

void Sphere::computeBoundingSphere(Matrix4 C)
{
	//compute the sphere
	bSphere.radius = radius;
	bSphere.center.set(C.multiplyVector(Vector4(0,0,0,1)));

	//compute the min and max vals
	bBox.min.set(C.multiplyVector(Vector4(-radius,-radius,-radius,1)));
	bBox.max.set(C.multiplyVector(Vector4( radius, radius, radius,1)));
}


/*******************************CUBE CLASS***********************************/

/*
 * Constructor. create a sphere with given radius, slices and stacks
 */
Cube::Cube(double size) : size(size)
{
	shaderEnabled = false;
}

/*
 * Destructor. create a sphere with given radius, slices and stacks
 */
Cube::~Cube()
{
	
}

/*
 * Draw the sphere
 */
void Cube::draw(Matrix4 C, Frustum F, bool checkCulling)
{
	Frustum::SphereCollision collState;
	if(checkCulling)
		collState = checkFrustumCulling(F);
	else
		collState = Frustum::INSIDE;

	//don't draw if sphere is outside
	if(collState != Frustum::OUTSIDE)
	{
		glLoadMatrixd(C.getPointer());
		glutSolidCube(size);
	}
}

/**************************RECTANGLE PRISM CLASS******************************/

/*
 * Constructor. create a sphere with given radius, slices and stacks
 */
RectanglePrism::RectanglePrism(double width, double length, double height) : width(width),
																			 length(length),
																			 height(height)
{
	shaderEnabled = false;
	rectangleScale.scale(width,height,length);
	
}

/*
 * Destructor. create a sphere with given radius, slices and stacks
 */
RectanglePrism::~RectanglePrism()
{
	
}

/*
 * Draw the rectangular prism
 */
void RectanglePrism::draw(Matrix4 C, Frustum F, bool checkCulling)
{
	Frustum::SphereCollision collState;
	if(checkCulling)
		collState = checkFrustumCulling(F);
	else
		collState = Frustum::INSIDE;

	//don't draw if sphere is outside
	if(collState != Frustum::OUTSIDE)
	{
		Matrix4 Cnew = C.multiplyBy(rectangleScale);
		glLoadMatrixd(Cnew.getPointer());
		glutSolidCube(1);
	}
}

/*
 * Compute the bounding box and sphere
 */
void RectanglePrism::computeBoundingSphere(Matrix4 C)
{
	double halfWidth  = width/2.0;
	double halfLength = length/2.0;
	double halfHeight = height/2.0;

	//compute the sphere
	//bSphere.radius = max( max(halfWidth,halfLength), halfHeight);
	bSphere.radius = Vector3(halfWidth,halfHeight,halfLength).magnitude();
	bSphere.center.set(C.multiplyVector(Vector4(0,0,0,1)));

	//compute the min and max vals
	bBox.min.set(C.multiplyVector(Vector4(-halfWidth,-halfHeight,-halfLength,1)));
	bBox.max.set(C.multiplyVector(Vector4( halfWidth, halfHeight, halfLength,1)));
}

/*******************************BEZIER CLASS**********************************/

//initizilze static constants
Matrix4 BezierCurve::B(-1,3,-3,1,3,-6,3,0,-3,3,0,0,1,0,0,0);
Matrix4 BezierCurve::Bnorm(-3,9,-9,3,6,-12,6,0,-3,3,0,0,0,0,0,0);

/*
 * Default constructor. For now creates a bezier curve with 7 control points
 * (2 attached curves).
 */
BezierCurve::BezierCurve()
{
	//create the default points
	controlPoints.push_back(new ControlPoint(Vector4(0.2320,10.5972,0,1),CPointType::INTERPOLATING));
	controlPoints.push_back(new ControlPoint(Vector4(5.8787,11.5254,0,1),CPointType::APPROXIMATING));
	controlPoints.push_back(new ControlPoint(Vector4(6.2655, 6.1108,0,1),CPointType::APPROXIMATING));

	controlPoints.push_back(new ControlPoint(Vector4(5.9561,-4.8731,0,1),CPointType::INTERPOLATING));
	controlPoints.push_back(new ControlPoint(Vector4(8.2766,-4.7184,0,1),CPointType::APPROXIMATING));
	controlPoints.push_back(new ControlPoint(Vector4(7.3483,-8.3540,0,1),CPointType::APPROXIMATING));

	controlPoints.push_back(new ControlPoint(Vector4(8.6634,-9.9783,0,1),CPointType::INTERPOLATING));
}

/*
 *
 */
BezierCurve::~BezierCurve()
{

}

/*
 * Draw the curve 
 */
void BezierCurve::draw(Matrix4 C, Frustum F, bool checkCulling)
{
	//draw the points
	glPointSize(10);
	glBegin(GL_POINTS);
	for(vector<ControlPoint*>::iterator it = controlPoints.begin(); it != controlPoints.end(); it++)
	{
		if((*it)->getPointType() == CPointType::INTERPOLATING)
		{
			glColor3f(0.4,0.4,1);
		}
		else
		{
			glColor3f(1,1,1);
		}
		
		glVertex4f((*it)->getPoint()[0],(*it)->getPoint()[1],(*it)->getPoint()[2],(*it)->getPoint()[3]);
	}
	glEnd();

	//draw the lines between the points
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(1,1,0);
	for(vector<ControlPoint*>::iterator it = controlPoints.begin(); it != controlPoints.end(); it++)
	{
		vector<ControlPoint*>::iterator next = it;
		next++;
		//draw if not at the last point
		if(next != controlPoints.end())
		{
			glVertex4f((*it)->getPoint()[0],(*it)->getPoint()[1],(*it)->getPoint()[2],(*it)->getPoint()[3]);
			glVertex4f((*next)->getPoint()[0],(*next)->getPoint()[1],(*next)->getPoint()[2],(*next)->getPoint()[3]);
		}
	}
	glEnd();

	//draw the curve
	int numSegments = (controlPoints.size()-1)/3;
	for(int i=0;i < numSegments; i++)
	{
		Vector4 p0 = controlPoints[i*3]->getPoint();
		Vector4 p1 = controlPoints[i*3+1]->getPoint();
		Vector4 p2 = controlPoints[i*3+2]->getPoint();
		Vector4 p3 = controlPoints[i*3+3]->getPoint();

		//calculate bezier const matrix
		Matrix4 G(p0[0],p0[1],p0[2],1,p1[0],p1[1],p1[2],1,p2[0],p2[1],p2[2],1,p3[0],p3[1],p3[2],1);
		Matrix4 Cbez = G.multiplyBy(B);
		Matrix4 Cnorm = G.multiplyBy(Bnorm);
		
		//calculate interpolated curve points and draw the lines
		Vector4 T;
		glBegin(GL_LINE_STRIP);
		glColor3f(0,1,1);
		for(double j=0; j <= (1.0 + (1.0/SUBDIVISIONS)); j += (1.0/(SUBDIVISIONS)))
		{
			T.setCoordinate((j*j*j),0);
			T.setCoordinate((j*j),1);
			T.setCoordinate(j,2);
			T.setCoordinate(1,3);

			T = Cbez.multiplyVector(T);
			glVertex3d(T[0],T[1],T[2]);
		}
		glEnd();
		
		/*
		//testing normals
		glBegin(GL_LINES);
		glColor3f(1,0,1);
		Vector4 Tprime;
		for(double j=0; j <= (1.0 + (1.0/SUBDIVISIONS)); j += (1.0/(SUBDIVISIONS)))
		{
			T.setCoordinate((j*j*j),0);
			T.setCoordinate((j*j),1);
			T.setCoordinate(j,2);
			T.setCoordinate(1,3);

			T = Cbez.multiplyVector(T);
			glVertex3d(T[0],T[1],T[2]);

			Tprime.setCoordinate((j*j),0);
			Tprime.setCoordinate((j),1);
			Tprime.setCoordinate(1,2);
			Tprime.setCoordinate(0,3);

			Tprime = Cnorm.multiplyVector(Tprime);
			Tprime.scale(3.0/(Tprime.magnitude()));
			glVertex3d(T[0] - Tprime[1], T[1] + Tprime[0], T[2]);
		}
		glEnd();
		*/
	}
}

/*
 * Compute the bounding box and sphere
 */
void BezierCurve::computeBoundingSphere(Matrix4 C)
{
	bBox.min.set( DBL_MAX,  DBL_MAX,  DBL_MAX, 1);
	bBox.max.set(-DBL_MAX, -DBL_MAX, -DBL_MAX, 1);

	//compute the box vals
	float setVals = false;
	for(vector<ControlPoint*>::iterator it = controlPoints.begin(); it != controlPoints.end(); it++)
	{
		setVals = true;
		if((*it)->getPoint().get(0) < bBox.min.get(0))
			bBox.min.setCoordinate((*it)->getPoint().get(0),0);
		if((*it)->getPoint().get(1) < bBox.min.get(1))
			bBox.min.setCoordinate((*it)->getPoint().get(1),1);
		if((*it)->getPoint().get(2) < bBox.min.get(2))
			bBox.min.setCoordinate((*it)->getPoint().get(1),2);
		if((*it)->getPoint().get(0) > bBox.max.get(0))
			bBox.min.setCoordinate((*it)->getPoint().get(0),0);
		if((*it)->getPoint().get(1) > bBox.max.get(1))
			bBox.min.setCoordinate((*it)->getPoint().get(1),1);
		if((*it)->getPoint().get(2) > bBox.max.get(2))
			bBox.min.setCoordinate((*it)->getPoint().get(1),2);
	}
	if(setVals == false)
	{
		bBox.min.set(0,0,0,1);
		bBox.max.set(0,0,0,1);
	}

	//compute the sphere
	bSphere.radius = (Vector4(bBox.min - bBox.max)).magnitude() / 2;
	bSphere.center.set( (bBox.min.get(0) + bBox.max.get(0)) / 2.0,
					    (bBox.min.get(1) + bBox.max.get(1)) / 2.0,
					    (bBox.min.get(2) + bBox.max.get(2)) / 2.0, 1 );
}

/*
 * Returns the index of a point at the given position. Takes a tolerance into
 * account to account for projection errors and mouse/screen accuracy. 
 */
int BezierCurve::getPointIndex(Vector4 pos, double tolerance)
{
	int pointNum = 0;
	for(vector<ControlPoint*>::iterator it = controlPoints.begin(); it != controlPoints.end(); it++)
	{
		Vector4 cPointPos((*it)->getPoint());

		if( abs(pos[0]-cPointPos[0]) <= tolerance && 
		    abs(pos[1]-cPointPos[1]) <= tolerance && 
			abs(pos[2]-cPointPos[2]) <= tolerance )
		{
			return pointNum;
		}
		pointNum++;
	}
	return -1;
}

/*
 * Move the point
 */
void BezierCurve::movePoint(Vector4 newPos, int index)
{
	controlPoints[index]->setPoint(newPos);
	controlPoints[index]->getPoint().print();
}

/*
 * create a new point
 */
BezierCurve::ControlPoint::ControlPoint(Vector4 p, CPointType pType)
{
	pos.set(p);
	type = pType;
}

/*
 * destroy a point
 */
BezierCurve::ControlPoint::~ControlPoint()
{

}

/*
 * get the points location
 */
Vector4 BezierCurve::ControlPoint::getPoint()
{
	return pos;
}

/*
 * set the point
 */
void BezierCurve::ControlPoint::setPoint(Vector4 p)
{
	pos.set(p);
}

/*
 * get the points type
 */
BezierCurve::ControlPoint::CPointType BezierCurve::ControlPoint::getPointType()
{
	return type;
}

/*
 * set the points type (may not ever be used
 */
void BezierCurve::ControlPoint::setPointType(CPointType pType)
{
	type = pType;
}

/************************SURFACE OF REVOLUTION CLASS**************************/

/*
 * Create a default surface
 */
RevolutionSurface::RevolutionSurface() : BezierCurve()
{
	rotDelta = 10.0;
	showTexture = false;
}

/*
 * create a surface with given rotation angle in degrees
 */
RevolutionSurface::RevolutionSurface(double angle)
{
	rotDelta = angle;
	showTexture = false;
}

/*
 * 
 */
RevolutionSurface::~RevolutionSurface()
{
	BezierCurve::~BezierCurve();
}

/*
 * draw the surface and draw the handles if enabled
 */
void RevolutionSurface::draw(Matrix4 C, Frustum F, bool checkCulling)
{
	//TODO add culling

	vector<Vector4> curPoint;
	vector<Vector4> nextPoint;

	//initialize the curve at idenity
	for(int i=0; i < controlPoints.size(); i++)
	{
		curPoint.push_back(controlPoints[i]->getPoint());
		nextPoint.push_back(Vector4(0,0,0,0));
	}
	rotation.reset();

	//iterate around the degrees of the surface
	for(double angle=0; angle <= 360; angle += rotDelta)
	{
		rotation.localRotateY(rotDelta * (M_PI/180));
		Matrix4 nextRotMatrix;
		Matrix4 rotMatrix;
		rotMatrix.rotateY(angle * (M_PI/180));
		nextRotMatrix.rotateY((angle + rotDelta) * (M_PI/180));
		
		//calculate the next points
		for(int i=0; i < curPoint.size(); i++)
			nextPoint[i] = rotMatrix.multiplyVector(controlPoints[i]->getPoint());

		//interpolate the curves and draw the surface
		int numSegments = (controlPoints.size()-1)/3;
		for(int i=0; i < numSegments; i++)
		{
			Vector4 p0Cur = curPoint[i*3];
			Vector4 p1Cur = curPoint[i*3+1];
			Vector4 p2Cur = curPoint[i*3+2];
			Vector4 p3Cur = curPoint[i*3+3];

			Vector4 p0Next = nextPoint[i*3];
			Vector4 p1Next = nextPoint[i*3+1];
			Vector4 p2Next = nextPoint[i*3+2];
			Vector4 p3Next = nextPoint[i*3+3];

			//calculate bezier const matrices
			Matrix4 GCur(p0Cur[0],p0Cur[1],p0Cur[2],1,
				         p1Cur[0],p1Cur[1],p1Cur[2],1,
					     p2Cur[0],p2Cur[1],p2Cur[2],1,
					     p3Cur[0],p3Cur[1],p3Cur[2],1);

			Matrix4 GNext(p0Next[0],p0Next[1],p0Next[2],1,
				          p1Next[0],p1Next[1],p1Next[2],1,
						  p2Next[0],p2Next[1],p2Next[2],1,
						  p3Next[0],p3Next[1],p3Next[2],1);

			Matrix4 GNorm(controlPoints[i*3]->getPoint()[0],controlPoints[i*3]->getPoint()[1],0,1,
						  controlPoints[i*3+1]->getPoint()[0],controlPoints[i*3+1]->getPoint()[1],0,1,
						  controlPoints[i*3+2]->getPoint()[0],controlPoints[i*3+2]->getPoint()[1],0,1,
						  controlPoints[i*3+3]->getPoint()[0],controlPoints[i*3+3]->getPoint()[1],0,1);

			Matrix4 CCur  = GCur.multiplyBy(B);
			Matrix4 CNext = GNext.multiplyBy(B);
			Matrix4 CNorm = GNorm.multiplyBy(Bnorm);

			glLoadMatrixd(C.getPointer());

			//draw the quads for the current strip
			Vector4 T, Tnorm, PCur, PNext, PCurNorm, PNextNorm;
			glBegin(GL_QUAD_STRIP);
			glColor3f(1,1,1);
			for(double j=0; j <= (1.0 + (1.0/SUBDIVISIONS)); j += (1.0/SUBDIVISIONS))
			{
				T.setCoordinate((j*j*j),0);
				T.setCoordinate((j*j),1);
				T.setCoordinate(j,2);
				T.setCoordinate(1,3);

				Tnorm.setCoordinate((j*j),0);
				Tnorm.setCoordinate((j),1);
				Tnorm.setCoordinate(1,2);
				Tnorm.setCoordinate(0,3);

				//calculate vertices
				PCur  = CCur.multiplyVector(T);
				PNext = CNext.multiplyVector(T);

				//calculate normals
				PCurNorm = CNorm.multiplyVector(Tnorm);					//calculate tangent
				PCurNorm.set(-PCurNorm[1],PCurNorm[0],0,0);				//get normal
				PCurNorm = rotMatrix.multiplyVector(PCurNorm);			//rotate normal
				PCurNorm.scale(1.0/PCurNorm.magnitude());				//normalize

				PNextNorm = CNorm.multiplyVector(Tnorm);				//calculate tangent
				PNextNorm.set(-PNextNorm[1],PNextNorm[0],0,0);			//get normal
				PNextNorm = nextRotMatrix.multiplyVector(PNextNorm);	//rotate normal
				PNextNorm.scale(1.0/PNextNorm.magnitude());				//normalize

				//calculate textures
				if(showTexture)
				{
					float texTCoord  = ((float)i * (float)SUBDIVISIONS + (float)j*SUBDIVISIONS) / 41;
					float texSCoord0 = (angle+80)/360.0;
					float texSCoord1 = (angle+80+rotDelta)/360.0;
					if(texSCoord0 > 1)
						texSCoord0 -= 1;
					if(texSCoord1 > 1)
						texSCoord1 -= 1;

					glTexCoord2f(texSCoord0,texTCoord);
					glNormal3d(PCurNorm[0],PCurNorm[1],PCurNorm[2]);
					glVertex3d(PCur[0],PCur[1],PCur[2]);

					glTexCoord2f(texSCoord1,texTCoord);
					glNormal3d(PNextNorm[0],PNextNorm[1],PNextNorm[2]);
					glVertex3d(PNext[0],PNext[1],PNext[2]);
				}
				else
				{
					glNormal3d(PCurNorm[0],PCurNorm[1],PCurNorm[2]);
					glVertex3d(PCur[0],PCur[1],PCur[2]);

					glNormal3d(PNextNorm[0],PNextNorm[1],PNextNorm[2]);
					glVertex3d(PNext[0],PNext[1],PNext[2]);
				}
			}
			glEnd();
			
			//SHOW NORMALS
			/*
			glBegin(GL_LINES);
			glColor3f(1,0,1);
			for(double j=0; j <= (1.0 + (1.0/SUBDIVISIONS)); j += (1.0/SUBDIVISIONS))
			{
				T.setCoordinate((j*j*j),0);
				T.setCoordinate((j*j),1);
				T.setCoordinate(j,2);
				T.setCoordinate(1,3);

				T = CCur.multiplyVector(T);
				glVertex3d(T[0],T[1],T[2]);

				Tnorm.setCoordinate((j*j),0);
				Tnorm.setCoordinate((j),1);
				Tnorm.setCoordinate(1,2);
				Tnorm.setCoordinate(0,3);

				Tnorm = CCurNorm.multiplyVector(Tnorm);
				Tnorm.scale(3.0/(Tnorm.magnitude()));
				if(T[0] < 0)
					glVertex3d(T[0] + Tnorm[1], T[1] - Tnorm[0], T[2]);
				else
					glVertex3d(T[0] - Tnorm[1], T[1] + Tnorm[0], T[2]);
			}
			glEnd();
			*/
		}

		for(int i=0; i < curPoint.size(); i++)
			curPoint[i] = nextPoint[i];

	}

	if(curveDisplayed)
	{
		glDepthFunc(GL_ALWAYS);
		BezierCurve::draw(C,F,checkCulling);
		glDepthFunc(GL_LESS);
	}
}

/*
 * TODO: I don't care about bounding stuff at the moment...but I probably should implement this.
 */
void RevolutionSurface::computeBoundingSphere(Matrix4 C)
{
	BezierCurve::computeBoundingSphere(C);

}

void RevolutionSurface::enableTexture()
{
	showTexture = true;
}

void RevolutionSurface::disableTexture()
{
	showTexture = false;
}

void RevolutionSurface::showCurve()
{
	curveDisplayed = true;
}

void RevolutionSurface::hideCurve()
{
	curveDisplayed = false;
}
