#include "RenderObject.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>

using namespace std;

/*
 * Create an empty render object
 */
RenderObject::RenderObject(void)
{
	colors = NULL;
	objLoaded = false;
}

/*
 * Create a new render object from the given filname
 */
RenderObject::RenderObject(char* filename)
{
	colors = NULL;
	loadObject(filename);
}

RenderObject::~RenderObject(void)
{

}

/*
 * Utility function to check if file exists
 */
bool RenderObject::fExists(char* filename)
{
	ifstream ifile(filename);
	return ifile.is_open();
}

/*
 * Loads a new object from file
 */
void RenderObject::loadObject(char* filename)
{
	if(fExists(filename))
	{
		ObjReader::readObj(filename,nVertices,&vertices,&normals,&texcoords,nIndices,&indices);
		//initialize the color array
		cout << "COLOR ARRAY: " << nVertices*3 << endl;
		if(colors != NULL)
		{
			delete[] colors;
			colors = new float[nVertices*3];
			
		}
		else
		{
			colors = new float[nVertices*3];
		}
		//set initial color to green
		setColor(0,1.0,0);
		
		objLoaded = true;
		cout << "Model " << filename << " loaded!" <<endl;
	}
	else
	{
		objLoaded = false;
		cout << "ERROR: Model " << filename << " not loaded" <<endl;
	}

}

/*
 * Loads an image as a heightmap in the vertex array
 */
void RenderObject::loadHeightmap(char* filename)
{
	
}

/*
 * Draw the object.
 */
void RenderObject::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(matrix.getGLModelview());
	mat.loadMaterial();

	//TODO: add checks for color, normal and texture arrays
	if(objLoaded)
	{
		if(colors != NULL)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, colors);
		}
		if(normals != NULL)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, normals);
		}
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);

		glDrawElements(GL_TRIANGLES,nVertices,GL_UNSIGNED_INT,indices);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}

/*
 * Sets the color of the object to a solid color
 */
void RenderObject::setColor(float r, float g, float b)
{
	for(int i=0; i < (nVertices*3); i++)
	{
		if((i % 3) == 0)
		{
			colors[i] = r;
		}
		else if( ( (i + 2) % 3) == 0)
		{
			colors[i] = g;
		}
		else if( ( (i + 1) % 3) == 0)
		{
			colors[i] = b;
		}
	}
}

/*
 * 
 */
void RenderObject::randColor()
{
	float lowerR, upperR, rInc, r;
	float lowerG, upperG, gInc, g;
	float lowerB, upperB, bInc, b;
	int rMod = rand() % 50+1;
	int rMod2 = rand() % 30+1;
	int gMod = rand() % 70+1;
	int gMod2 = rand() % 30+1;
	int bMod = rand() % 80+1;
	int bMod2 = rand() % 30+1;

	//randomize our upper and lower color limits
	lowerR = (float(rand() % rMod))/50;
	upperR = (float(rand() % rMod2)+(50-rMod2))/50;
	r = lowerR;
	rInc = (upperR - lowerR) / (float)nVertices*3;

	lowerG = (float(rand() % gMod2)+(70-gMod2))/70;
	upperG = (float(rand() % gMod))/50;
	g = lowerG;
	gInc = (upperG - lowerG) / (float)nVertices*3;

	lowerB = (float(rand() % bMod))/50;
	upperB = (float(rand() % bMod2)+(80-bMod2))/80;
	b = lowerB;
	bInc = (upperB - lowerB) / (float)nVertices*3;

	for(int i=0; i < (nVertices*3); i++)
	{
		if((i % 3) == 0)
		{
			if(i == 0)
				r = lowerR;
			else
				r += rInc;

			//colors[i] = r;
			colors[i] = ((float)((rand() % 100)+1)) / 100.0;
		}
		else if( ( (i + 2) % 3) == 0)
		{
			if(i == 0)
				g = lowerG;
			else
				g += gInc;

			//colors[i] = g;
			colors[i] = ((float)((rand() % 100)+1)) / 100.0;
		}
		else if( ( (i + 1) % 3) == 0)
		{
			if(i == 0)
				b = lowerB;
			else
				b += bInc;

			//colors[i] = b;
			colors[i] = ((float)((rand() % 100)+1)) / 100.0;
		}
	}
}

float RenderObject::getMinX()
{
	if(objLoaded)
	{
		float min = vertices[0];
		for(int i = 3; i < (nVertices * 3); i += 3)
		{
			if(vertices[i] < min)
				min = vertices[i];
		}
		return min;
	}
	else
	{
		return 0;
	}
}

float RenderObject::getMinY()
{
	if(objLoaded)
	{
		float min = vertices[0];
		for(int i = 4; i < (nVertices * 3); i += 3)
		{
			if(vertices[i] < min)
				min = vertices[i];
		}
		return min;
	}
	else
	{
		return 0;
	}
}

float RenderObject::getMinZ()
{
	if(objLoaded)
	{
		float min = vertices[2];
		for(int i = 5; i < (nVertices * 3); i += 3)
		{
			if(vertices[i] < min)
				min = vertices[i];
		}
		cout << "Min Z: " << min << endl;
		return min;
	}
	else
	{
		return 0;
	}
}

float RenderObject::getMaxX()
{
	if(objLoaded)
	{
		float max = vertices[0];
		for(int i = 3; i < (nVertices * 3); i += 3)
		{
			if(vertices[i] > max)
				max = vertices[i];
		}
		return max;
	}
	else
	{
		return 0;
	}
}

float RenderObject::getMaxY()
{
	if(objLoaded)
	{
		float max = vertices[0];
		for(int i = 4; i < (nVertices * 3); i += 3)
		{
			if(vertices[i] > max)
				max = vertices[i];
		}
		return max;
	}
	else
	{
		return 0;
	}
}

float RenderObject::getMaxZ()
{
	if(objLoaded)
	{
		float max = vertices[2];
		for(int i = 5; i < (nVertices * 3); i += 3)
		{
			if(vertices[i] > max)
				max = vertices[i];
		}
		return max;
	}
	else
	{
		return 0;
	}
}

