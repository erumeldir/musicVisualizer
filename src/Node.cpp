#include "Node.h"
#include <stdio.h>
#include <iostream>

using namespace std;

/***************************NODE BASE CLASS METHODS****************************/

/*
 * set the node's name
 */
void Node::setName(char* nodeName)
{
	name = nodeName;
}

/*
 * get the node name
 */
char* Node::getName()
{
	return name;
}

/*
 * Set the node's parent
 */
void Node::setParent(Node* nodeParent)
{
	parent = nodeParent;
}

/*
 * get node's parent
 */
Node* Node::getParent()
{
	return parent;
}

Frustum::SphereCollision Node::checkFrustumCulling(Frustum camFrustum)
{
	Vector3 sphereCenter(bSphere.center.get(0),bSphere.center.get(1),bSphere.center.get(2));
	int collisionState;

	return camFrustum.sphereInFrustum(sphereCenter,bSphere.radius);
	
	//camFrustum.sphereInFrustum(
	/*SphereCollision collType;
	Vector3 fSpaceCenter;
	Vector4 fSpaceVector;
	double fSpaceRadius;

	double sphereDist;

	//convert everything to frustum space
	fSpaceVector = projection.multiplyVector(bSphere.center);
	fSpaceVector.dehomogenize();
	fSpaceCenter = Vector3(fSpaceVector.get(0),fSpaceVector.get(1),fSpaceVector.get(2));

	//radius is the same at x, y or z
	//scale by projection matrix
	fSpaceVector = projection.multiplyVector(Vector4(bSphere.radius,0,0,1));
	fSpaceVector.dehomogenize();
	fSpaceRadius = fSpaceVector.get(0);

	//begin checking
	int containsCounter = 0;

	//check the +X direction
	sphereDist = fSpaceCenter.dot(Vector3(1,0,0)) - 1;
	if(sphereDist > fSpaceRadius)
	{
		return OUTSIDE;
	}
	else if(sphereDist < -fSpaceRadius)
	{
		containsCounter++;
	}

	//check the +Y direction
	sphereDist = fSpaceCenter.dot(Vector3(0,1,0)) - 1;
	if(sphereDist > fSpaceRadius)
	{
		return OUTSIDE;
	}
	else if(sphereDist < -fSpaceRadius)
	{
		containsCounter++;
	}

	//check the +Z direction
	sphereDist = fSpaceCenter.dot(Vector3(0,0,1)) - 1;
	if(sphereDist > fSpaceRadius)
	{
		return OUTSIDE;
	}
	else if(sphereDist < -fSpaceRadius)
	{
		containsCounter++;
	}

	//check the -X direction
	sphereDist = fSpaceCenter.dot(Vector3(-1,0,0)) - 1;
	if(sphereDist > fSpaceRadius)
	{
		return OUTSIDE;
	}
	else if(sphereDist < -fSpaceRadius)
	{
		containsCounter++;
	}

	//check the -Y direction
	sphereDist = fSpaceCenter.dot(Vector3(0,-1,0)) - 1;
	if(sphereDist > fSpaceRadius)
	{
		return OUTSIDE;
	}
	else if(sphereDist < -fSpaceRadius)
	{
		containsCounter++;
	}

	//check the -Z direction
	sphereDist = fSpaceCenter.dot(Vector3(0,0,-1)) - 1;
	if(sphereDist > fSpaceRadius)
	{
		return OUTSIDE;
	}
	else if(sphereDist < -fSpaceRadius)
	{
		containsCounter++;
	}

	if(containsCounter == 6)
	{
		return INSIDE;
	}
	else
	{
		return INTERSECT;
	}*/
}

/*****************************GROUP CLASS METHODS*****************************/

/*
 * add a child to the end of the list
 */
void Group::addChild(Node* child)
{
	children.push_back(child);
	children.back()->setParent(this);
}



/*****************************GEODE CLASS METHODS*****************************/

/*
 * set the material
 */
void Geode::setMaterial(Material* material)
{
	mat = material;
}

/*
 * set the shader
 *
void Geode::setShader(Shader* newShader)
{
	shader = newShader;
}*/

/*
 * enable the shader
 */
void Geode::enableShader()
{
	shaderEnabled = true;
}

/*
 * disable the shader
 */
void Geode::disableShader()
{
	shaderEnabled = false;
}

/***************************MATRIX TRANSFORM CLASS*******************************/

/*
 * Constructor
 */
MatrixTransform::MatrixTransform()
{
	name = NULL;
	reset();
}

MatrixTransform::~MatrixTransform()
{
	//TODO add virtual destructors
	//destroy the children
	for(list<Node*>::iterator it = children.begin(); it != children.end(); it++)
	{
		delete (*it);		
	}
}

/*
 * Apply the matrix transform and draw all the children
 */
void MatrixTransform::draw(Matrix4 C, Frustum F, bool checkCulling)
{
	//apply the matrix transform
	Matrix4 Cnew = C.multiplyBy(getMatrix());

	Frustum::SphereCollision collState;
	if(checkCulling)
		collState = checkFrustumCulling(F);
	else
		collState = Frustum::INSIDE;

	//draw all the child nodes
	for(list<Node*>::iterator it = children.begin(); it != children.end(); it++)
	{
		if(collState == Frustum::OUTSIDE)
		{
			printf("%s culled\n",name);
			break;
		}
		else if(collState == Frustum::INSIDE)
		{
			(*it)->draw(Cnew,F,false);
		}
		else
		{
			(*it)->draw(Cnew,F,true);
		}
		
	}
}

/*
 * compute the group's bounding sphere
 */
void MatrixTransform::computeBoundingSphere(Matrix4 C)
{
	//apply the matrix transform
	Matrix4 Cnew = C.multiplyBy(getMatrix());
	//draw all the child nodes

	bBox.min.set(10000,10000,10000,1);
	bBox.max.set(-10000,-10000,-10000,1);

	for(list<Node*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->computeBoundingSphere(Cnew);

		//compute groups bounding box
		if((*it)->bBox.min.get(0) < bBox.min.get(0))
			bBox.min.setCoordinate((*it)->bBox.min.get(0),0);
		if((*it)->bBox.min.get(1) < bBox.min.get(1))
			bBox.min.setCoordinate((*it)->bBox.min.get(1),1);
		if((*it)->bBox.min.get(2) < bBox.min.get(2))
			bBox.min.setCoordinate((*it)->bBox.min.get(2),2);
		if((*it)->bBox.max.get(0) > bBox.max.get(0))
			bBox.max.setCoordinate((*it)->bBox.max.get(0),0);
		if((*it)->bBox.max.get(1) > bBox.max.get(1))
			bBox.max.setCoordinate((*it)->bBox.max.get(1),1);
		if((*it)->bBox.max.get(2) > bBox.max.get(2))
			bBox.max.setCoordinate((*it)->bBox.max.get(2),2);
	}

	//compute group bounding sphere
	bSphere.radius = (Vector4(bBox.min - bBox.max)).magnitude() / 2;
	bSphere.center.set( (bBox.min.get(0) + bBox.max.get(0)) / 2.0,
					    (bBox.min.get(1) + bBox.max.get(1)) / 2.0,
					    (bBox.min.get(2) + bBox.max.get(2)) / 2.0, 1 );

}