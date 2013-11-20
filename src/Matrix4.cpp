#include <math.h>
#include <iostream>
#include <stdio.h>
#include "Matrix4.h"

using namespace std;

Matrix4::Matrix4()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }
} 
Matrix4::Matrix4(
  double m00, double m01, double m02, double m03,
  double m10, double m11, double m12, double m13,
  double m20, double m21, double m22, double m23,
  double m30, double m31, double m32, double m33 )
{
  m[0][0] = m00;
  m[0][1] = m01;
  m[0][2] = m02;
  m[0][3] = m03;
  m[1][0] = m10;
  m[1][1] = m11;
  m[1][2] = m12;
  m[1][3] = m13;
  m[2][0] = m20;
  m[2][1] = m21;
  m[2][2] = m22;
  m[2][3] = m23;
  m[3][0] = m30;
  m[3][1] = m31;
  m[3][2] = m32;
  m[3][3] = m33;
}

double* Matrix4::getPointer()
{
  return &m[0][0];
}

/*
 * get
 * takes in two ints denoting the row/column for the element and returns
 * the value stored in the matrix.
*/
double Matrix4::get(int x, int y){
  return m[x][y];
}

/*
* set
* takes in two ints denoting the row/column for the element and sets
* the value stored in the matrix.
*/
void Matrix4::set(int x, int y, double value){
  m[x][y] = value;
}

/*
 * set
 * takes in matrix and makes this matrix's values that of the
 * other matrix
 */
void Matrix4::set(Matrix4 other){
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      m[i][j] = other.get(i, j);
    }
  }
}

/*
 * set
 * takes in matrix and makes this matrix's values that of the
 * array of doubles
 */
void Matrix4::set(float* mOther){
  int k = 0;

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      m[i][j] = mOther[k];
	  k++;
    }
  }
}

// multiplies this matrix by another matrix
void Matrix4::multiply(Matrix4 other)
{
  // temporary matrix for final result
  Matrix4 result(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

  // iterating over our final matrix
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      double value = 0;
      // computing dot product for our current position
      for (int k = 0; k < cols; k++)
      {
        value += get(k,j) * other.get(i,k);
      }
      result.set(i, j, value);
    }
  }

  // set this matrix to be the result matrix
  set(result);
}

// multiplies this matrix by another matrix and returns the result
Matrix4 Matrix4::multiplyBy(Matrix4 other)
{
  // temporary matrix for final result
  Matrix4 result(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

  // iterating over our final matrix
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      double value = 0;
      // computing dot product for our current position
      for (int k = 0; k < cols; k++)
      {
        value += get(k,j) * other.get(i,k);
      }
      result.set(i, j, value);
    }
  }

  // set this matrix to be the result matrix
  return result;
}

// multiples matrix by a vector
Vector4 Matrix4::multiplyVector(Vector4 vector)
{
  Vector4 result(0, 0, 0, 0);
  // iterating over matrix
  for (int i = 0; i < rows; i++)
  {
    double value = 0;
    for (int j = 0; j < cols; j++)
    {
      value += get(j,i) * vector.get(j);
    }
    result.setCoordinate(value, i);
  }

  return result;
}

void Matrix4::identity()
{
  double ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = ident[i][j];
    }
  }
}

// rotate around the X axis by the angle in radians
void Matrix4::rotateX(double angle)
{
  m[0][0] = 1;
  m[0][1] = 0;
  m[0][2] = 0;
  m[0][3] = 0;
  m[1][0] = 0;
  m[1][1] = cos(angle);
  m[1][2] = sin(angle);
  m[1][3] = 0;
  m[2][0] = 0;
  m[2][1] = -sin(angle);
  m[2][2] = cos(angle);
  m[2][3] = 0;
  m[3][0] = 0;
  m[3][1] = 0;
  m[3][2] = 0;
  m[3][3] = 1;
}

// rotate around the Y axis by the angle in radians
void Matrix4::rotateY(double angle)
{
  m[0][0] = cos(angle);
  m[0][1] = 0;
  m[0][2] = sin(angle);
  m[0][3] = 0;
  m[1][0] = 0;
  m[1][1] = 1;
  m[1][2] = 0;
  m[1][3] = 0;
  m[2][0] = -sin(angle);
  m[2][1] = 0;
  m[2][2] = cos(angle);
  m[2][3] = 0;
  m[3][0] = 0;
  m[3][1] = 0;
  m[3][2] = 0;
  m[3][3] = 1;
}

// rotate around the Z axis by the angle in radians
void Matrix4::rotateZ(double angle)
{
	m[0][0] = cos(angle);
	m[0][1] = sin(angle);
	m[0][2] = 0;
	m[0][3] = 0;
	m[1][0] = -sin(angle);
	m[1][1] = cos(angle);
	m[1][2] = 0;
	m[1][3] = 0;
	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 1;
	m[2][3] = 0;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

// rotate around the vector by the angle in radians
void Matrix4::rotate(double angle, Vector3 vector)
{
  vector.normalize();

  double x = vector.get(0);
  double y = vector.get(1);
  double z = vector.get(2);

  m[0][0] = cos(angle) + x*x*(1 - cos(angle));
  m[0][1] = y*x*(1-cos(angle)) + z*sin(angle);
  m[0][2] = z*x*(1-cos(angle)) - y*sin(angle);
  m[0][3] = 0;
  m[1][0] = x*y*(1-cos(angle)) - z*sin(angle);
  m[1][1] = cos(angle) + y*y*(1-cos(angle));
  m[1][2] = z*y*(1-cos(angle)) + x*sin(angle);
  m[1][3] = 0;
  m[2][0] = x*z*(1-cos(angle)) + y*sin(angle);
  m[2][1] = y*z*(1-cos(angle)) - x*sin(angle);
  m[2][2] = cos(angle) + z*z*(1-cos(angle));
  m[2][3] = 0;
  m[3][0] = 0;
  m[3][1] = 0;
  m[3][2] = 0;
  m[3][3] = 1;
}

// turns the matrix into scaling matrix
void Matrix4::scale(double sx, double sy, double sz)
{
  m[0][0] = sx;
  m[0][1] = 0;
  m[0][2] = 0;
  m[0][3] = 0;
  m[1][0] = 0;
  m[1][1] = sy;
  m[1][2] = 0;
  m[1][3] = 0;
  m[2][0] = 0;
  m[2][1] = 0;
  m[2][2] = sz;
  m[2][3] = 0;
  m[3][0] = 0;
  m[3][1] = 0;
  m[3][2] = 0;
  m[3][3] = 1;
}

// turns matrix into translation matrix
void Matrix4::translate(double tx, double ty, double tz)
{
  m[0][0] = 1;
  m[0][1] = 0;
  m[0][2] = 0;
  m[0][3] = 0;
  m[1][0] = 0;
  m[1][1] = 1;
  m[1][2] = 0;
  m[1][3] = 0;
  m[2][0] = 0;
  m[2][1] = 0;
  m[2][2] = 1;
  m[2][3] = 0;
  m[3][0] = tx;
  m[3][1] = ty;
  m[3][2] = tz;
  m[3][3] = 1;
}

// turns matrix into translation matrix
void Matrix4::translate(Vector3 other)
{
  m[0][0] = 1;
  m[0][1] = 0;
  m[0][2] = 0;
  m[0][3] = 0;
  m[1][0] = 0;
  m[1][1] = 1;
  m[1][2] = 0;
  m[1][3] = 0;
  m[2][0] = 0;
  m[2][1] = 0;
  m[2][2] = 1;
  m[2][3] = 0;
  m[3][0] = other.get(0);
  m[3][1] = other.get(1);
  m[3][2] = other.get(2);
  m[3][3] = 1;
}

// turns matrix into translation matrix
void Matrix4::transpose()
{
  // temp matrix to store the result
  Matrix4 result(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  
  // iterate over matrix
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // store m into n of result
      result.set(i, j, get(j,i));
    }
  }

  // sets this matrix to the result
  set(result);
}

void Matrix4::print()
{
  cout << "Matrix4:" << endl;
  // iterate over matrix
  for (int i = 0; i < rows; i++)
  {
    printf(" | ");
    for (int j = 0; j < cols; j++)
    {
      printf("%.4f ", get(j,i));
    }
    printf("|\n");
  }
  cout << endl;
}

/*
 * Computes the inverse of a generic 4x4 matrix
 */
Matrix4& Matrix4::inverse(const double tolerance)
{
	//calculate submatrix determinants
	//double S0  =
	return Matrix4();
}