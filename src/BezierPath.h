#ifndef _BEZIERPATH_H_
#define _BEZIERPATH_H_

#include "BezierSegment.h"
#include <vector>
#include <math.h>

class BezierPath
{
protected:
	std::vector<BezierSegment*> curves; // The curves for this path

public:
	BezierPath();
	~BezierPath();

	void addCurve(BezierSegment*); // add a BezierSegment to the end of the path
};

#endif