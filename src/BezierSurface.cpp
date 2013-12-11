#include "BezierSurface.h"

BezierSurface::BezierSurface(int bands, int time, double pSize, double xScal, double yScal, double zScal)
{
  // variables dealing with the number of patches
  numPatches = bands - 1;
  maxTime = time;
  // variables dealing with size and scale of patches
  patchSize = pSize;
  xScale = xScal;
  yScale = yScal;
  zScale = zScal;

  // create multi dimensional array of bezier patches
  surface = new BezierPatch4*[numPatches];
  for (int i = 0; i < numPatches; i++)
    surface[i] = new BezierPatch4[time];
  
  // upper and left bounds of the xz plane
  double left = numPatches / 2.0 * -1 * patchSize*xScale;
  double top = maxTime/2.0*-1 * patchSize*zScale;

  // initialization of each patch (start at height zero)
  for (int i = 0; i < numPatches; i++)
  {
    for (int j = 0; j < maxTime; j++)
    {
      // upper and lower bounds of the current patch
      Vector3 topLeft(left + i*patchSize*xScale, 0, top + j*patchSize*zScale);
      Vector3 topRight(left + (i + 1)* patchSize*xScale, 0, top + j*patchSize*zScale);
      Vector3 bottom(left + i*patchSize*xScale, 0, top + (j + 1)* patchSize*zScale);

      // make initial Bezier patch square 
      surface[i][j] = BezierPatch4(topLeft, topRight, bottom);
    }
  }
}

BezierSurface::~BezierSurface()
{
  // delete array of objects
  delete[] surface;
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
bool BezierSurface::addBand(float * amps)
{
  // push all the bands back
  pushBack();

  // add the new band to the bottom row
  for (int i = 0; i < numPatches-1; i++)
  {
    // set amplitude of patches
    BezierPatch4* current = &(surface[i][maxTime - 1]);
    BezierPatch4* nextPatch = &(surface[i + 1][maxTime - 1]);
    current->setAmplitude(amps[i] * yScale);
    nextPatch->setAmplitude(amps[i + 1] * yScale);

    // join patches of the new band together
    BezierPatch4* topPatch = &(surface[i][maxTime - 2]);
    surface[i][maxTime - 1].join(topPatch, nextPatch);
  }

  // join the last band
  surface[numPatches - 1][maxTime - 1].join(&(surface[numPatches - 1][maxTime - 2]), 0, amps[numPatches]);

  // second pass for C1 continuity
  for (int i = 0; i < numPatches - 1; i++)
  {
    // patch above the current one
    BezierPatch4* topPatch = &(surface[i][maxTime - 2]);

    // far left patch
    if (i == 0)
    {
      surface[i][maxTime - 1].joinCont(topPatch, 0);
    }
    else
    {
      // patch left of the current
      BezierPatch4* leftPatch = &(surface[i - 1][maxTime - 1]);
      // join patches of the new band together
      surface[i][maxTime - 1].joinCont(topPatch, leftPatch);
    }
  }

  return true;
}

/*
 * pushBack
 *
 * Push all of the rows back one and zero out amplitudes of the front row
 */
void BezierSurface::pushBack()
{
  // move all of the patches back a row on the z plane
  for (int i = 0; i < numPatches; i++)
  {
    for (int j = 0; j < maxTime-1; j++)
    {
      // get the current and next patch for their amplitudes
      BezierPatch4 currPatch = surface[i][j];
      BezierPatch4 nextPatch = surface[i][j + 1];
      // get all the y values from the next patch
      currPatch.copyAmplitude(nextPatch);

      // move all of the rows back
      surface[i][j] = currPatch;
    }
  }

  // upper and left bounds of the xz plane
  double left = numPatches / 2.0 * -1 * patchSize*xScale;
  double top = maxTime / 2.0*-1 * patchSize*zScale;

  // zero out the front row
  for (int i = 0; i < numPatches; i++)
  {
    Vector3 topLeft(left + i*patchSize*xScale, 0, top + (maxTime - 1)*patchSize*zScale);
    Vector3 topRight(left + (i + 1)* patchSize*xScale, 0, top + (maxTime - 1)*patchSize*zScale);
    Vector3 bottom(left + i*patchSize*xScale, 0, top + (maxTime)* patchSize*zScale);

    // make initial Bezier patch square 
    surface[i][maxTime - 1] = BezierPatch4(topLeft, topRight, bottom);
  }
}