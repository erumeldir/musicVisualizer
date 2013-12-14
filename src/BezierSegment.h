#ifndef _BezierSegment_H_
#define _BezierSegment_H_

#include <stdlib.h>

class BezierSegment
{
protected:
	float ctrlpoints[4][2];    // array of 4 control points
	double amplitude;               // y direction height based on the band

public:
	BezierSegment();                          // default constructor
	BezierSegment(float[2], float[2]);		  // constructor that makes a bezier curve that is a straight line
	BezierSegment(float[2], float[2], float[2], float[2]); // constructor that takes all 4 control points for a BezierSegment
	~BezierSegment();

	void setControlPoints(float[2], float[2], float[2], float[2]); // takes values for all 4 control points
	void setControlPoint(int, float[2]); // changes the value of a single control point

	float getX(float t);
	float getY(float t);
};

#endif