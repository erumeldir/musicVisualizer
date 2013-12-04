#include <stdlib.h>
#include "GL\glut.h"
#include "Geometry.h"

/******************************************************************************
 * BezierPatch4
 * 
 * This class describes an order 4 Bezier Patch. You can create a patch by
 * giving a list of 16 (4x4) control points in the form of Vector3s. These 
 * control points may also be changed dynamically through the setter methods.
 * 
 ******************************************************************************/
class BezierPatch4: public Geode
{
protected:
  GLfloat ctrlpoints[4][4][3];    // 4x4 grid of control points

public:
  BezierPatch4(Vector3*);      // constructor that takes an array to EXACTLY 16 Vector3s.. or else!
  ~BezierPatch4();

  void setControlPoints(Vector3*);  // takes array to 16 Vector3s and changes the control points
  void setControlPoint(int,int,Vector3); // changes the value of a single control point

  void draw(Matrix4 C, Frustum F, bool checkCulling); // draws the bezier curve
  void computeBoundingSphere(Matrix4 C);
};

