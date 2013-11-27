#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "Vector4.h"

class Vector3
{
protected:
  double m[3];   // vector elements
  static const int rows = 3;

public:
  Vector3();        // constructor
  Vector3(double, double, double);
  ~Vector3();
  void set(double, double, double);  // sets the coordniates of the vector to the given values
  void setCoordinate(double, int);  // sets the X coordniates of the vector to the given values
  void setVector(Vector3&); // sets coordinates to match given vector
  void operator =(Vector3& other) { setVector(other); }
  void setVector(Vector4&); // sets coordinates to match given vector
  void operator =(Vector4& other) { setVector(other); }

  double get(int); // gets a particular value of a coordinate
  double operator [](int i) { return get(i); } // operator overload for brackets

  Vector3 add( Vector3&);  // adds given vector to this vector
  Vector3 operator +( Vector3& other) { return add(other); }// operator overload for +
  void addTo(Vector3&);
  void operator +=(Vector3& other) { return addTo(other); }

  Vector3 sub( Vector3&);  // subtracts given vector from this vector
  Vector3 operator -( Vector3& other) { return sub(other); } // operator overload for -
  void subBy(Vector3&);
  void operator -=(Vector3& other) { return subBy(other); }

  void negate(); // negates the vector
  void scale(double); // scales vector by the given value

  double dot(Vector3&); // computes dot product of this vector and the given vector
  Vector3 cross(Vector3&); // computes the cross product of this vector and the given vector

  double magnitude(); // computes the magnitude
  double angle(Vector3&);

  float* getPtrf();

  void normalize(); // normalizes (creats the unit vector)
  void print(); // displays vector to standard output
};


#endif