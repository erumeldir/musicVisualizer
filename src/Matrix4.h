#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "Vector4.h"
#include "Vector3.h"

class Matrix4
{
  protected:
    double m[4][4];   // matrix elements
    static const int rows = 4;
    static const int cols = 4;
    
  public:
    Matrix4();        // constructor
    Matrix4(double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double);
    double* getPointer();  // return pointer to matrix elements
    double get(int, int);  // gets the value at the given place in the matrix
    void set(int, int, double); // sets the value at the given place
    void set(Matrix4); // sets the matrix's values to that of the given matrix
	void set(float*);	//sets the matrix's values to a 16 element array of doubles
    void operator =(Matrix4 other) { set(other); }  // operator overload for = sety
    void multiply(Matrix4);   // multiplies by matrix and modifies calling matrix
	Matrix4 multiplyBy(Matrix4);	//multiplies by the other matrix and returns the result
    void operator *(Matrix4 other) { multiply(other); }  // operator overload for * multiply
    Vector4 multiplyVector(Vector4);  // multiplies by vector and returns a vectors
    void identity();  // create identity matrix
    void rotateX(double); // rotation about x axis in radians
    void rotateY(double); // rotation about y axis in radians
    void rotateZ(double); // rotation about z axis in radians
    void rotate(double, Vector3); // rotation about an arbitrary axis in radians
    void scale(double, double, double);  // uniform scale
    void translate(double, double, double);  // translate that takes in 3 doubles
    void translate(Vector3);  // translate that takes in vector3
    void transpose();  // transposes the matrix (switches m and n)
    void print();  // prints the matrix
	Matrix4& inverse(const double);	//computes a generic inverse
};

#endif