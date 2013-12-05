/*
 * BezierSurface
 *
 * Bezier surface used to describe the FFT bands of an audio file. 
 * Creates a surface with a given number of audio bands and creates N-1
 * continuous patches to represent the bands. Any time a row of FFT
 * amplitudes is added, all of the previously represented values are
 * pushed back and the new row is represented in front. All rows are
 * continuous with the rows in front and in back. The surface will
 * only store T rows in the Z direction where T represents the "time"
 * or how long the row has been in the surface.
 */

#include "BezierPatch4.h"

#define patchSize 2

class BezierSurface : public Geode
{
protected:
  int numPatches; // how many patches we want to represent; numBands - 1 is how many patches we need 
  int maxTime;  // how many rows in the Z direction we want
  BezierPatch4** surface; // multidimensional array of Bezier patches representing the surface

public:
  BezierSurface(int bands, int time);
  ~BezierSurface();

  // getter for a patch

  bool addBand(double*); // an array of "numBands" number of bands that gets added to the front
  void pushBack(); // pushes all of the rows of the surface back a row and zeros out the front row

  // overidden Geode methods
  void draw(Matrix4 C, Frustum F, bool checkCulling); // draws the bezier surface
  void computeBoundingSphere(Matrix4 C);
};

