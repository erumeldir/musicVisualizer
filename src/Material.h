#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Vector4.h"

class Material
{
protected:
	Vector4 ambientColor;		//ambient color of the material
	Vector4 diffuseColor;		//diffuse color of the material (primary color of object)
	Vector4 specularColor;		//specular color of the material
	Vector4 emissionColor;		//color of simulated light emission
	float shininess[1];			//size and intensity of specular highlight [0.0,128.0] 
								//higher number = smaller and more intense highlight
	bool useObjColor;			//if enabled, will use the object color for diffuse and ambient light
public:
	Material();									//create material with default properties
	Material(Vector4);							//create material with given diffuse color
	Material(Vector4,Vector4,Vector4);			//create material with given ambient, diffuse and specular values
	Material(Vector4,Vector4,Vector4,float);	//create material with ambient, diffuse, specular and shininess

	void setAmbient(Vector4);		//set ambient color
	void setDiffuse(Vector4);		//set diffuse color
	void setSpecular(Vector4);		//set specular color
	void setEmission(Vector4);		//set emission color
	void setShininess(float);		//set shininess

	void loadMaterial();			//loads the material into GL buffers	
	void useObjectColor(bool);		//set whether or not to use the object color for diffuse/ambient mat settings
};

#endif