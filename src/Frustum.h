/*
 * Code taken from Lighthouse3d
 * http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-implementation/
 */

#include "Vector3.h"

#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

typedef struct FrustumPlane
{
	Vector3 normal;
	double d;
};

class Frustum
{
private:
	//enum for different planes
	enum
	{
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		GL_NEAR,
		GL_FAR
	};

public:
	//enum for different circle intersections
	typedef enum SphereCollision
	{
		OUTSIDE,
		INSIDE,
		INTERSECT
	}SphereCollision;

	FrustumPlane planes[6];

	//frustum vertices
	Vector3	nearTopLeft;
	Vector3	nearTopRight;
	Vector3	nearBottomLeft;
	Vector3	nearBottomRight;
	Vector3	farTopLeft;
	Vector3	farTopRight;
	Vector3	farBottomLeft;
	Vector3	farBottomRight;

	float nearDist;		//near plane
	float farDist;		//far plane
	float ratio;		//aspect
	float dang;			//angle in deg
	float tang;			//tangent of angle

	float nearWidth;	//width of near plane
	float nearHeight;	//height of near plane
	float farWidth;		//width of far plane
	float farHeight;	//height of far plane

	Frustum();
	~Frustum();

	void setCamInternals(float angle, float aspect, float nearD, float farD);
	void setCamDef(Vector3 &p, Vector3 &look, Vector3 &up);
	//int pointInFrustm(Vector3 &p);
	SphereCollision sphereInFrustum(Vector3 &p, float ratio);
};

#endif