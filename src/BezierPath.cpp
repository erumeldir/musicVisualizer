#include "BezierPath.h"

void BezierPath::addCurve(BezierSegment* curve)
{
	curves.push_back(curve);
}