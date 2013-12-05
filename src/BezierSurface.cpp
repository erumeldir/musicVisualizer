#include "BezierSurface.h"

BezierSurface::BezierSurface(int bands, int time, double pSize, double hScale)
{
  numPatches = bands - 1;
  maxTime = time;
  patchSize = pSize;
  heightScale = hScale;

  // create multi dimensional array of patches
  surface = new BezierPatch4*[numPatches];
  for (int i = 0; i < numPatches; i++)
    surface[i] = new BezierPatch4[time];
  
  // upper and left bounds of the xz plane
  double left = numPatches / 2.0 * -1 * patchSize;
  double top = maxTime/2.0*-1 * patchSize;

  // initialization
  for (int i = 0; i < numPatches; i++)
  {
    for (int j = 0; j < maxTime; j++)
    {
      // upper and lower bounds of the current patch
      Vector3 topLeft(left + i*patchSize, 0, top + j*patchSize);
      Vector3 topRight(left + (i +1)* patchSize, 0, top + j*patchSize);
      Vector3 bottom(left + i*patchSize, 0, top + (j+1)* patchSize);

      // make initial Bezier patch square 
      surface[i][j] = BezierPatch4(topLeft, topRight, bottom);
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
  // draw every patch
  for(int i = 0; i < numPatches; i++)
  {
    for (int j = 0; j < maxTime; j++)
    {
      // make initial Bezier patch square 
      surface[i][j].draw( C,  F,  checkCulling);
    }
  }
  glPopMatrix();
}

// TODO: finish writing this
void BezierSurface::computeBoundingSphere(Matrix4 C)
{
}

/*
 * addBand
 * 
 * Takes in an array of numPatches + 1 doubles as the "amplitudes" for the
 * surface. Changes the frontmost row to be the new amplitudes and pushes
 * all the other rows back one (like a queue). All patches on the surface
 * are meant to be C1 continuous.
 * 
 * returns true if successful otherwise false
 */
bool BezierSurface::addBand(double * amps)
{
  // push all the bands back
  pushBack();

  // add the new band to the bottom row
  // zero out the last row
  for (int i = 0; i < numPatches; i++)
  {
    BezierPatch4 current = surface[i][maxTime - 1];
    current.setAmplitude(amps[i]*heightScale);

    // make initial Bezier patch square 
    surface[i][maxTime - 1] = current;
  }

  // join them together
  for (int i = 0; i < numPatches-1; i++)
  {
    // get the current and next patch for their amplitudes
    BezierPatch4 nextPatch = surface[i+1][maxTime - 1];
    // get patches above
    BezierPatch4 topPatch = surface[i][maxTime - 2];
    BezierPatch4 cornerPatch = surface[i + 1][maxTime - 2];

    surface[i][maxTime - 1].join(topPatch, cornerPatch, nextPatch);
  }

  return true;
}

/*
 * pushBack
 *
 * Push all of the rows back one and zero out the front row
 */
void BezierSurface::pushBack()
{
  for (int i = 0; i < numPatches; i++)
  {
    for (int j = 0; j < maxTime-1; j++)
    {
      // get the current and next patch for their amplitudes
      BezierPatch4 currPatch = surface[i][j];
      BezierPatch4 nextPatch = surface[i][j + 1];
      currPatch.setAmplitude(nextPatch.getAmplitude());
      // get all the y values from the next patch
      currPatch.copyAmplitude(nextPatch);

      // move all of the rows back
      surface[i][j] = currPatch;
    }
  }


  // upper and left bounds of the xz plane
  double left = numPatches / 2.0 * -1 * patchSize;
  double top = maxTime / 2.0*-1 * patchSize;

  // zero out the last row
  for (int i = 0; i < numPatches; i++)
  {
    Vector3 topLeft(left + i*patchSize, 0, top + (maxTime - 1)*patchSize);
    Vector3 topRight(left + (i + 1)* patchSize, 0, top + (maxTime - 1)*patchSize);
    Vector3 bottom(left + i*patchSize, 0, top + (maxTime)* patchSize);

    // make initial Bezier patch square 
    surface[i][maxTime - 1] = BezierPatch4(topLeft, topRight, bottom);
  }
}