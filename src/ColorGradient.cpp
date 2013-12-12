#include "ColorGradient.h"

ColorGradient::ColorGradient()
{
	dx = 1.0;
}

ColorGradient::~ColorGradient()
{

}

void ColorGradient::addColor(Vector4 c)
{
	colors.push_back(c);
	if(colors.size() > 1)
		dx = 1.0  / ((float)colors.size()-1.0);
}

void ColorGradient::addColor(Vector3 c)
{
	colors.push_back(Vector4(c[0],c[1],c[2],1.0));
	if(colors.size() > 1)
		dx = 1.0  / ((float)colors.size()-1.0);
}

Vector4 ColorGradient::getColor(float x)
{
	//handle empty gradients
	if(colors.size() == 0)
	{
		return Vector4(1.0,1.0,1.0,1.0);
	}
	else if(colors.size() == 1)
	{
		return colors[0];
	}

	int p0;
	int p1;

	for(p1 = 1; p1 < colors.size(); p1++)
	{
		p0 = p1 - 1;
		if((float)p1*dx > x)
			break;
	}

	Vector4 finalColor;
	finalColor = colors[p1] - colors[p0];
	finalColor.scale((x-p0*dx)/dx);
	finalColor = finalColor + colors[p0];

	return finalColor;
}