#define _USE_MATH_DEFINES
#include <math.h>

#include "Frustum.h"

Frustum::Frustum()
{

}

Frustum::~Frustum()
{

}

void Frustum::setCamInternals(float angle, float aspect, float nearD, float farD)
{
	ratio = aspect;
	dang = angle;
	nearDist = nearD;
	farDist = farD;

	//compute width and height of near/far planes
	tang = (float)tan((M_PI/180.0) * dang * 0.5);
	nearHeight = nearDist * tang;
	nearWidth = nearHeight * ratio;
	farHeight = farDist * tang;
	farWidth = farHeight * ratio;
}

void Frustum::setCamDef(Vector3 &p, Vector3 &look, Vector3 &up)
{
	Vector3 dir, nearCenter, farCenter, X, Y, Z;

	Z = p - look;
	Z.normalize();

	X = up.cross(Z);
	X.normalize();

	Y = Z.cross(X);

	//compute the centers
	Z.scale(nearDist);
	nearCenter = p - Z;
	Z.normalize();
	Z.scale(farDist);
	farCenter = p - Z;
	Z.normalize();

	//compute the near corners
	Y.scale(nearHeight);
	X.scale(nearWidth);
	nearTopLeft     = nearCenter + Y - X;
	nearTopRight    = nearCenter + Y + X;
	nearBottomLeft  = nearCenter - Y - X;
	nearBottomRight = nearCenter - Y + X;
	Y.normalize();
	X.normalize();

	//compute the far corners
	Y.scale(nearHeight);
	X.scale(nearWidth);
	farTopLeft     = farCenter + Y - X;
	farTopRight    = farCenter + Y + X;
	farBottomLeft  = farCenter - Y - X;
	farBottomRight = farCenter - Y + X;
	Y.normalize();
	X.normalize();

	//compute the planes
	Vector3 v0, v1;
	Y.scale(nearHeight);
	X.scale(nearWidth);

	//top
	v0 = (farTopLeft - nearTopRight);
	v1 = (nearTopLeft - nearTopRight);
	planes[TOP].normal = v0.cross(v1);
	planes[TOP].normal.normalize();
	planes[TOP].d = planes[TOP].normal.dot(nearCenter + Y);

	//bottom
	v0 = (farBottomLeft - nearBottomLeft);
	v1 = (nearBottomRight - nearBottomLeft);
	planes[BOTTOM].normal = v0.cross(v1);
	planes[BOTTOM].normal.normalize();
	planes[BOTTOM].d = planes[BOTTOM].normal.dot(nearCenter - Y);

	//left
	v0 = (farTopLeft - nearTopLeft);
	v1 = (nearBottomLeft - nearTopLeft);
	planes[LEFT].normal = v0.cross(v1);
	planes[LEFT].normal.normalize();
	planes[LEFT].d = planes[LEFT].normal.dot(nearCenter - X);

	//right
	v0 = (nearBottomRight - nearTopRight);
	v1 = (farTopRight - nearTopRight);
	planes[RIGHT].normal = v0.cross(v1);
	planes[RIGHT].normal.normalize();
	planes[RIGHT].d = planes[RIGHT].normal.dot(nearCenter + X);

	//near
	planes[GL_NEAR].normal = Z;
	planes[GL_NEAR].normal.normalize();
	planes[GL_NEAR].d = planes[GL_NEAR].normal.dot(nearCenter);

	//far
	Z.scale(-1);
	planes[GL_FAR].normal = Z;
	planes[GL_FAR].normal.normalize();
	planes[GL_FAR].d = planes[GL_FAR].normal.dot(nearCenter);
}

//int pointInFrustm(Vector3 &p)

Frustum::SphereCollision Frustum::sphereInFrustum(Vector3 &p, float radius)
{
	float distance;
	SphereCollision result = INSIDE;

	for(int i=0; i < 6; i++)
	{
		//compute the distance to the plane
		distance = p.dot(planes[i].normal) - planes[i].d;
		if(distance > radius)
			return OUTSIDE;
		else if(distance > -radius)
			result = INTERSECT;
	}
	return(result);
}