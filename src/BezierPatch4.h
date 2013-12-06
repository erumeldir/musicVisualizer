#include <stdlib.h>
#include "glut.h"
#include "Geometry.h"

#define NUM_CONTROL_POINTS 16

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
  double amplitude;               // y direction height based on the band

public:
  BezierPatch4();      // default constructor
  BezierPatch4(Vector3, Vector3, Vector3); // constructor that takes the corners and makes a flat patch
  BezierPatch4(Vector3*);      // constructor that takes an array to EXACTLY 16 Vector3s.. or else!
  ~BezierPatch4();

  void setControlPoints(Vector3*);  // takes array to 16 Vector3s and changes the control points
  void setControlPoint(int, int, Vector3); // changes the value of a single control point
  Vector3 getControlPoint(int, int); // gets the value of a single control point

  // Sound amplitude functions
  void copyAmplitude(BezierPatch4); // copies the amplitude of the given patch
  void setAmplitude(double);  // sets the amplitude of the patch (the y value representing the sound)
  double getAmplitude(); // gets the amplitude of a patch
  void join(BezierPatch4* top, BezierPatch4* right, double edgeAmp = 0); // connects the current patch to the adjacent ones


  void draw(Matrix4 C, Frustum F, bool checkCulling); // draws the bezier curve
  void computeBoundingSphere(Matrix4 C);
};

