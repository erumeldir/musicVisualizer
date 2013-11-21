#ifndef _NODE_H_
#define _NODE_H_

#include <list>
#include "Material.h"
#include "ModelMatrix.h"
#include "Vector4.h"
#include "Frustum.h"

using namespace std;

/******************************************************************************
 * The base node class
 ******************************************************************************/
class Node
{
protected:
	char* name;		//name of the node
	Node* parent;	//the node's parent

public:
	virtual void draw(Matrix4,Frustum,bool) = 0;
	virtual void computeBoundingSphere(Matrix4) = 0;

	//bounding sphere in world coords
	struct Boundingsphere {
		Vector4 center;
		double radius;
	}bSphere;

	//bounding box
	struct BoundingBox {
		Vector4 min;
		Vector4 max;
	}bBox;

	//check frustum
	Frustum::SphereCollision checkFrustumCulling(Frustum);

	//get and set functions
	void setName(char*);
	char* getName();
	void setParent(Node*);
	Node* getParent();
};

/******************************************************************************
 * The base group class
 ******************************************************************************/
class Group : public Node
{
protected:
	list<Node*> children;

public:
	void addChild(Node*);
};

/******************************************************************************
 * The base geode class
 ******************************************************************************/
class Geode : public Node
{
protected:
	Material* mat;
	//Shader* shader;
	bool shaderEnabled;

public:
	void setMaterial(Material*);
	//void setShader(Shader*);
	void enableShader();
	void disableShader();
};

/******************************************************************************
 * The matrix transform class. Is a group sublcass and matrix4
 ******************************************************************************/
class MatrixTransform : public Group, public ModelMatrix
{
protected:

public:
	//constructor
	MatrixTransform();
	~MatrixTransform();

	void draw(Matrix4,Frustum,bool); //set the next matrix and draw the child nodes
	void computeBoundingSphere(Matrix4);
};

#endif