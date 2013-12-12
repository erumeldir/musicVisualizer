#ifndef _COLOR_GRADIENT_H_
#define _COLOR_GRADIENT_H_

#include <vector>
#include "Vector4.h"
#include "Vector3.h"

using namespace std;

class ColorGradient
{
private:
	vector<Vector4> colors;
	float dx;

public:
	ColorGradient();
	~ColorGradient();

	void addColor(Vector4);
	void addColor(Vector3);

	Vector4 getColor(float);
};

#endif