#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>

#include "ModelMatrix.h"

class Light
{
protected:

	//light color
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	
	//perform transposition of the light
	ModelMatrix matrix;

	//positions
	Vector3 direction;
	Vector4 position;

	//keeps track of light number
	int lightNumber;

	//light init
	virtual void init() {}

public:
	//direction getters and setters
	void setDirection(double x,double y,double z) { direction.set(x,y,z); }
	void setDirection(Vector3 d) { direction.setVector(d); }
	Vector3 getDirection() { return direction; }

	//position getters and setters
	virtual void setPosition(double,double,double) {}
	virtual void setPosition(Vector3) {}
	Vector3 getPosition() { return Vector3(position[0],position[1],position[2]); }

	//change the point the light looks at
	void pointAt(double x, double y, double z) { direction = Vector3(x,y,z) - Vector3(position[0],position[1],position[2]); }
	void pointAt(Vector3 p) { direction = p - Vector3(position[0],position[1],position[2]); }
	void pointAt(Vector4 p) { direction = Vector3(p[0],p[1],p[2]) - Vector3(position[0],position[1],position[2]); }

	//light intensity getters and setters
	void setAmbientColor(Vector4 color)  { ambient  = color; }
	void setDiffuseColor(Vector4 color)  { diffuse  = color; }
	void setSpecularColor(Vector4 color) { specular = color; }

	//update and create the light with properties in world
	virtual void update() {}

	//enable/disable
	void enable();
	void disable();

	//get the modelmatrix
	ModelMatrix& getMatrix() { return matrix; }
};

class DirectionalLight : public Light
{
public:
	//constructors
	DirectionalLight(int);
	DirectionalLight(int,Vector4,Vector4,Vector4);
	~DirectionalLight();

	void init();
	void update();

	void setPosition(double,double,double);
	void setPosition(Vector3);
};

class PointLight : public Light
{
protected:
	float constAtten;
	float linearAtten;
	float quadAtten;

public:
	//constructors
	PointLight(int);
	PointLight(int,Vector4,Vector4,Vector4);
	~PointLight();

	void init();
	void update();

	void setPosition(double,double,double);
	void setPosition(Vector3);

	void setAttenuation(float,float,float);
};

class SpotLight : public Light
{
protected:
	float beamWidth;		//angle of the beam
	float falloff;			//exponential falloff param, higher = sharper light falloff

	//attenuation parameters
	float constAtten;
	float linearAtten;
	float quadAtten;

public:
	//constructors
	SpotLight(int);
	SpotLight(int,double,double);
	SpotLight(int,Vector4,Vector4,Vector4);
	SpotLight(int,double,double,Vector4,Vector4,Vector4);
	~SpotLight();

	void init();
	void update();

	void setPosition(double,double,double);
	void setPosition(Vector3);

	//get and set
	void setBeamWidth(double angle) { beamWidth = angle; }
	double getBeamWidth() { return beamWidth; }
	void setFalloff(double f) { falloff = f; }
	double getFalloff() { return falloff; }
	void setAttenuation(float,float,float);
};

#endif