#include "BezierSegment.h"

BezierSegment::BezierSegment()
{
    for (int i = 0; i < 4; i++)
    {
        ctrlpoints[i][0] = 0.0;
        ctrlpoints[i][1] = 0.0;
    }
}

BezierSegment::~BezierSegment()
{

}


BezierSegment::BezierSegment(float p1[2], float p2[2])
{

	float dX = p2[0] - p1[0];
	float dY = p2[1] - p1[1];

	for (int i = 0; i < 4; i++)
	{
		ctrlpoints[i][0] = p1[0] + i*dX/3.0;
		ctrlpoints[i][1] = p1[1] + i*dY/3.0;
	}
}

BezierSegment::BezierSegment(float p1[2], float p2[2], float p3[2], float p4[2])
{  
	ctrlpoints[0][0] = p1[0];
	ctrlpoints[0][1] = p1[1];

	ctrlpoints[1][0] = p2[0];
	ctrlpoints[1][1] = p2[1];

	ctrlpoints[2][0] = p3[0];
	ctrlpoints[2][1] = p3[1];

	ctrlpoints[3][0] = p4[0];
	ctrlpoints[3][1] = p4[1];
}

void BezierSegment::setControlPoints(float p1[2], float p2[2], float p3[2], float p4[2])
{
	ctrlpoints[0][0] = p1[0];
	ctrlpoints[0][1] = p1[1];

	ctrlpoints[1][0] = p2[0];
	ctrlpoints[1][1] = p2[1];

	ctrlpoints[2][0] = p3[0];
	ctrlpoints[2][1] = p3[1];

	ctrlpoints[3][0] = p4[0];
	ctrlpoints[3][1] = p4[1];
}


void BezierSegment::setControlPoint(int n, float p[2])
{
	ctrlpoints[n][0] = p[0];
	ctrlpoints[n][1] = p[1];
}

float BezierSegment::getX(float t)
{
	float a_x, a_y, b_x, b_y, c_x, c_y, d_x, d_y;
	a_x = -1*ctrlpoints[0][0] + 3*ctrlpoints[1][0] - 3*ctrlpoints[2][0] + ctrlpoints[3][0];
	a_y = -1*ctrlpoints[0][1] + 3*ctrlpoints[1][1] - 3*ctrlpoints[2][1] + ctrlpoints[3][1];
	b_x = 3*ctrlpoints[0][0] - 6*ctrlpoints[1][0] + 3*ctrlpoints[2][0];
	b_y = 3*ctrlpoints[0][1] - 6*ctrlpoints[1][1] + 3*ctrlpoints[2][1];
	c_x = -3*ctrlpoints[0][0] + 3*ctrlpoints[1][0];
	c_y = -3*ctrlpoints[0][1] + 3*ctrlpoints[1][1];
	d_x = ctrlpoints[0][0];
	d_y = ctrlpoints[0][1];
	return a_x*t*t*t + b_x*t*t + c_x*t + d_x;
}

float BezierSegment::getY(float t)
{
	float a_x, a_y, b_x, b_y, c_x, c_y, d_x, d_y;
	a_x = -1*ctrlpoints[0][0] + 3*ctrlpoints[1][0] - 3*ctrlpoints[2][0] + ctrlpoints[3][0];
	a_y = -1*ctrlpoints[0][1] + 3*ctrlpoints[1][1] - 3*ctrlpoints[2][1] + ctrlpoints[3][1];
	b_x = 3*ctrlpoints[0][0] - 6*ctrlpoints[1][0] + 3*ctrlpoints[2][0];
	b_y = 3*ctrlpoints[0][1] - 6*ctrlpoints[1][1] + 3*ctrlpoints[2][1];
	c_x = -3*ctrlpoints[0][0] + 3*ctrlpoints[1][0];
	c_y = -3*ctrlpoints[0][1] + 3*ctrlpoints[1][1];
	d_x = ctrlpoints[0][0];
	d_y = ctrlpoints[0][1];
	return a_y*t*t*t + b_y*t*t + c_y*t + d_y;
}