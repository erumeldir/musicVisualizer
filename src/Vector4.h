#ifndef _VECTOR4_H_
#define _VECTOR4_H_

class Vector4
{
protected:
  double m[4];                               // vector elements
  static const int rows = 4;

public:
  Vector4();                                 // constructor
  Vector4(double, double, double);           // constructor 3 params
  Vector4(double, double, double, double);   // constructor 4 params
  ~Vector4();
  void set(double, double, double, double);  // sets the coordniates of the vector to the given values
  void set(Vector4&);                        // sets coordinates to match given vector
  void setCoordinate(double, int);           // sets the X coordniates of the vector to the given values
  void operator =(Vector4& other) { set(other); }
  double get(int);                           // gets a particular value of a coordinate
  double operator [](int i) { return get(i); } // operator overload for brackets
  Vector4 add(Vector4&);                     // adds given vector to this vector
  Vector4 operator +(Vector4& other) { return add(other); }// operator overload for +
  void addTo(Vector4&);
  void operator +=(Vector4& other) { return addTo(other); }
  Vector4 sub(Vector4&);                     // subtracts given vector from this vector
  Vector4 operator -(Vector4& other) { return sub(other); } // operator overload for -
  void subBy(Vector4&);
  void operator -=(Vector4& other) { return subBy(other); }
  void dehomogenize();                       // divides everything by the fourth component
  void negate();                             // negates the vector
  void scale(double);                        // scales vector by the given value
  double magnitude();                        // computes the magnitude
  //double* getPtrd() { return &m; }
  float*  getPtrf();	
  void print();                              // displays vector to standard output
};


#endif