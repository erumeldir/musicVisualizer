#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <GL\glut.h>
#include <math.h>
#include "Light.h"

/*********************************LIGHT CLASS*********************************/

/*
 * Enable the light in GL
 */
void Light::enable()
{
	glEnable(lightNumber);
}

/*
 * Disable the light in GL
 */
void Light::disable()
{
	glDisable(lightNumber);
}

/******************************DIRECTIONAL LIGHT******************************/

/*
 * Default directional light values
 */
void DirectionalLight::init()
{
	ambient.set(0,0,0,1);
	diffuse.set(1,1,1,1);
	specular.set(1,1,1,1);
	position.set(0,0,1,0);
	direction.set(0,0,-1);
}

/*
 * create a directional light with light num
 */
DirectionalLight::DirectionalLight(int lightNum)
{
	lightNumber = lightNum;
	init();
}

/*
 * Create a directional light with light num and init 
 */
DirectionalLight::DirectionalLight(int lightNum, Vector4 ambient, Vector4 diffuse, Vector4 specular)
{
	lightNumber = lightNum;
	init();
	setAmbientColor(ambient);
	setDiffuseColor(diffuse);
	setSpecularColor(specular);
}

DirectionalLight::~DirectionalLight()
{

}

/*
 * update the light settings
 */
void DirectionalLight::update()
{
	glLoadMatrixd(matrix.getGLModelview());
	glLightfv(lightNumber, GL_AMBIENT, ambient.getPtrf());
	glLightfv(lightNumber, GL_DIFFUSE, diffuse.getPtrf());
	glLightfv(lightNumber, GL_SPECULAR, specular.getPtrf());
	glLightfv(lightNumber, GL_SPOT_DIRECTION, direction.getPtrf());
	glLightfv(lightNumber, GL_POSITION, position.getPtrf());
	glLightf(lightNumber, GL_SPOT_EXPONENT, 0);
	glLightf(lightNumber, GL_SPOT_CUTOFF, 180);
	glLightf(lightNumber, GL_CONSTANT_ATTENUATION, 1);
	glLightf(lightNumber, GL_LINEAR_ATTENUATION, 0);
	glLightf(lightNumber, GL_QUADRATIC_ATTENUATION, 0);
}

/*
 * Set the position
 */
void DirectionalLight::setPosition(double x, double y, double z)
{
	position.set(x,y,z,0);
}

/*
 * set the position
 */
void DirectionalLight::setPosition(Vector3 pos)
{
	position.set(pos[0],pos[1],pos[2],0);
}

/*********************************SPOTLIGHT***********************************/

/*
 * Default directional light values
 */
void SpotLight::init()
{
	ambient.set(0,0,0,1);
	diffuse.set(1,1,1,1);
	specular.set(1,1,1,1);
	position.set(0,0,1,1);
	direction.set(0,0,-1);
	constAtten = 0;
	linearAtten = 0;
	quadAtten = 0.2;
	beamWidth = 30;
	falloff = 16;
}

/*
 * create a directional light with light num
 */
SpotLight::SpotLight(int lightNum)
{
	lightNumber = lightNum;
	init();
}

SpotLight::SpotLight(int lightNum, double cuttoffAngle, double falloffParam)
{
	lightNumber = lightNum;
	init();
	beamWidth = cuttoffAngle;
	falloff = falloffParam;
}

SpotLight::SpotLight(int lightNum, double cuttoffAngle, double falloffParam, Vector4 ambient, Vector4 diffuse, Vector4 specular)
{
	lightNumber = lightNum;
	init();
	beamWidth = cuttoffAngle;
	falloff = falloffParam;
	setAmbientColor(ambient);
	setDiffuseColor(diffuse);
	setSpecularColor(specular);
}

/*
 * Create a directional light with light num and init 
 */
SpotLight::SpotLight(int lightNum, Vector4 ambient, Vector4 diffuse, Vector4 specular)
{
	lightNumber = lightNum;
	init();
	setAmbientColor(ambient);
	setDiffuseColor(diffuse);
	setSpecularColor(specular);
}

SpotLight::~SpotLight()
{

}

/*
 * update the light settings
 */
void SpotLight::update()
{
	glLoadMatrixd(matrix.getGLModelview());
	glLightfv(lightNumber, GL_AMBIENT, ambient.getPtrf());
	glLightfv(lightNumber, GL_DIFFUSE, diffuse.getPtrf());
	glLightfv(lightNumber, GL_SPECULAR, specular.getPtrf());
	glLightfv(lightNumber, GL_SPOT_DIRECTION, direction.getPtrf());
	glLightfv(lightNumber, GL_POSITION, position.getPtrf());
	glLightf(lightNumber, GL_SPOT_EXPONENT, falloff);
	glLightf(lightNumber, GL_SPOT_CUTOFF, beamWidth);
	glLightf(lightNumber, GL_CONSTANT_ATTENUATION, constAtten);
	glLightf(lightNumber, GL_LINEAR_ATTENUATION, linearAtten);
	glLightf(lightNumber, GL_QUADRATIC_ATTENUATION, quadAtten);
}

/*
 * Set the position
 */
void SpotLight::setPosition(double x, double y, double z)
{
	position.set(x,y,z,1);
}

/*
 * set the position
 */
void SpotLight::setPosition(Vector3 pos)
{
	position.set(pos[0],pos[1],pos[2],1);
}

void SpotLight::setAttenuation(float constant, float linear, float quadratic)
{
	constAtten = constant;
	linearAtten = linear;
	quadAtten = quadratic;
}

/********************************POINT LIGHT**********************************/

/*
 * Default directional light values
 */
void PointLight::init()
{
	ambient.set(0,0,0,1);
	diffuse.set(1,1,1,1);
	specular.set(1,1,1,1);
	position.set(0,0,1,1);
	direction.set(0,0,-1);
	constAtten = 0;
	linearAtten = 0;
	quadAtten = 0.2;
}

/*
 * create a directional light with light num
 */
PointLight::PointLight(int lightNum)
{
	lightNumber = lightNum;
	init();
}

/*
 * Create a directional light with light num and init 
 */
PointLight::PointLight(int lightNum, Vector4 ambient, Vector4 diffuse, Vector4 specular)
{
	lightNumber = lightNum;
	init();
	setAmbientColor(ambient);
	setDiffuseColor(diffuse);
	setSpecularColor(specular);
}

PointLight::~PointLight()
{

}

/*
 * update the light settings
 */
void PointLight::update()
{
	glLoadMatrixd(matrix.getGLModelview());
	glLightfv(lightNumber, GL_AMBIENT, ambient.getPtrf());
	glLightfv(lightNumber, GL_DIFFUSE, diffuse.getPtrf());
	glLightfv(lightNumber, GL_SPECULAR, specular.getPtrf());
	glLightfv(lightNumber, GL_POSITION, position.getPtrf());
	glLightfv(lightNumber, GL_SPOT_DIRECTION, direction.getPtrf());
	glLightf(lightNumber, GL_SPOT_EXPONENT, 0);
	glLightf(lightNumber, GL_SPOT_CUTOFF, 180);
	glLightf(lightNumber, GL_CONSTANT_ATTENUATION, constAtten);
	glLightf(lightNumber, GL_LINEAR_ATTENUATION, linearAtten);
	glLightf(lightNumber, GL_QUADRATIC_ATTENUATION, quadAtten);
}

/*
 * Set the position
 */
void PointLight::setPosition(double x, double y, double z)
{
	position.set(x,y,z,1);
}

/*
 * set the position
 */
void PointLight::setPosition(Vector3 pos)
{
	position.set(pos[0],pos[1],pos[2],1);
}

void PointLight::setAttenuation(float constant, float linear, float quadratic)
{
	constAtten = constant;
	linearAtten = linear;
	quadAtten = quadratic;
}