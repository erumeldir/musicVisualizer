#include "BezierPatch4.h"

// Bezier patch constructor
BezierPatch4::BezierPatch4(Vector3 * controlPoints)
{
  // convert control points into array of floats
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      Vector3 currentVec = controlPoints[i*4 + j];
      ctrlpoints[i][j][0] = currentVec.get(0);
      ctrlpoints[i][j][1] = currentVec.get(1);
      ctrlpoints[i][j][2] = currentVec.get(2);
    }
  }

  // load control points
  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
    0, 1, 12, 4, &ctrlpoints[0][0][0]);
  glEnable(GL_MAP2_VERTEX_3);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
}


// takes array to 16 Vector3s and changes the control points
void BezierPatch4::setControlPoints(Vector3* controlPoints)
{

}

// changes the value of a single control point
void BezierPatch4::setControlPoint(int m, int n, Vector3 vec)
{
  ctrlpoints[m][n][0] = vec.get(0);
  ctrlpoints[m][n][1] = vec.get(1);
  ctrlpoints[m][n][2] = vec.get(2);
}

// draws the bezier curve
void BezierPatch4::draw(Matrix4 C, Frustum F, bool checkCulling)
{
  // load control points
  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
    0, 1, 12, 4, &ctrlpoints[0][0][0]);

  // draw curve
  glLoadMatrixd(C.getPointer());
  glRotatef(85.0, 1.0, 1.0, 1.0);
  glEvalMesh2(GL_FILL, 0, 20, 0, 20);
}

// TODO: finish writing this
void BezierPatch4::computeBoundingSphere(Matrix4 C)
{
}