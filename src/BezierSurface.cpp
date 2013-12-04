#include "BezierSurface.h"

BezierSurface::BezierSurface(int bands, int time)
{
  numBands = bands;
  maxTime = time;

  // create multi dimensional array of patches
  BezierPatch4** rows = new BezierPatch4*[numBands];
  for (int i = 0; i < numBands; i++)
    rows[i] = new BezierPatch4[time];
  
  // upper and lower bounds of the xz plane
  double left = numBands/2*-1 * patchSize;
  double right = numBands/2* patchSize;
  double top = time/2*-1 * patchSize;
  double bottom = time/2 * patchSize;

  // initialization
  for (int i = 0; i < numBands; i++)
  {
    for (int j = 0; j < maxTime; j++)
    {
      // upper and lower bounds of the current patch

    }
  }
}

BezierSurface::~BezierSurface()
{
  // TODO: delete your array
}

// draws the bezier surface. Initially it is a flat surface
void BezierSurface::draw(Matrix4 C, Frustum F, bool checkCulling)
{
  // load matrix 
  glLoadMatrixd(C.getPointer());
  glPopMatrix();
}

// TODO: finish writing this
void BezierSurface::computeBoundingSphere(Matrix4 C)
{
}