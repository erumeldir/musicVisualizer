#ifndef _RENDER_OBJECT_
#define _RENDER_OBJECT_

#include "ModelMatrix.h"
#include "objreader.h"
#include "Material.h"

//Contains everything to render an obj object
class RenderObject
{
private:
	bool fExists(char*);	//util function to check if file exists
protected: 
	ModelMatrix matrix;		//object matrix
	
	//The total number of verts and faces
	int nVertices;
	int nIndices;
	int width;
	int height;

	//the object's material
	Material mat;

	//The necessary arrays for the object
	float* vertices;
	float* normals;
	float* texcoords;
	float* colors;
	int*   indices;

	bool objLoaded;	//check if safe to draw

public:
	RenderObject(void);			//create a blank render object
	RenderObject(char*);		//Create a render object from file
	~RenderObject(void);

	void loadObject(char*);				//load a new object from file
	void loadHeightmap(char*);			//load a height map from a png image
	void draw();						//draw the object
	void setColor(float,float,float);	//make the object a solid color
	void randColor();					//make each vertex a random color

	float getMinX();
	float getMinY();
	float getMinZ();
	float getMaxX();
	float getMaxY();
	float getMaxZ();

	ModelMatrix& getMatrix() { return matrix; }	//return the model matrix

	Material& getMaterial() { return mat; }		//return the material
};

#endif