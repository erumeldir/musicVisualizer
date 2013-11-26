#include "ShaderGroup.h"

/***************************SHADER GROUP CLASS*******************************/
/*
* Constructor
*/
ShaderGroup::ShaderGroup(Shader* shad)
{
  shader = shad;
}

ShaderGroup::~ShaderGroup()
{
  //TODO add virtual destructors
  //destroy the children
  for (list<Node*>::iterator it = children.begin(); it != children.end(); it++)
  {
    delete (*it);
  }
}

/*
* Apply the matrix transform and draw all the children
*/
void ShaderGroup::draw(Matrix4 C, Frustum F, bool checkCulling)
{
  shader->getPid();
  // turn on shader
  //draw all the child nodes
  shader->bind();
  for (list<Node*>::iterator it = children.begin(); it != children.end(); it++)
  {
    (*it)->draw(C, F, checkCulling);
  }
  // turn off shader
  shader->unbind();
}

/*
* compute the group's bounding sphere
*/
void ShaderGroup::computeBoundingSphere(Matrix4 C)
{
  //draw all the child nodes

  bBox.min.set(10000, 10000, 10000, 1);
  bBox.max.set(-10000, -10000, -10000, 1);

  for (list<Node*>::iterator it = children.begin(); it != children.end(); it++)
  {
    (*it)->computeBoundingSphere(C);

    //compute groups bounding box
    if ((*it)->bBox.min.get(0) < bBox.min.get(0))
      bBox.min.setCoordinate((*it)->bBox.min.get(0), 0);
    if ((*it)->bBox.min.get(1) < bBox.min.get(1))
      bBox.min.setCoordinate((*it)->bBox.min.get(1), 1);
    if ((*it)->bBox.min.get(2) < bBox.min.get(2))
      bBox.min.setCoordinate((*it)->bBox.min.get(2), 2);
    if ((*it)->bBox.max.get(0) > bBox.max.get(0))
      bBox.max.setCoordinate((*it)->bBox.max.get(0), 0);
    if ((*it)->bBox.max.get(1) > bBox.max.get(1))
      bBox.max.setCoordinate((*it)->bBox.max.get(1), 1);
    if ((*it)->bBox.max.get(2) > bBox.max.get(2))
      bBox.max.setCoordinate((*it)->bBox.max.get(2), 2);
  }

  //compute group bounding sphere
  bSphere.radius = (Vector4(bBox.min - bBox.max)).magnitude() / 2;
  bSphere.center.set((bBox.min.get(0) + bBox.max.get(0)) / 2.0,
    (bBox.min.get(1) + bBox.max.get(1)) / 2.0,
    (bBox.min.get(2) + bBox.max.get(2)) / 2.0, 1);

}