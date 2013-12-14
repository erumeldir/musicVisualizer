#include "Vector3.h"
#include <cmath>
#include <iostream>

using namespace std;

// default construct, sets coords to [0,0,0]
Vector3::Vector3()
{
  m[0] = 0;
  m[1] = 0;
  m[2] = 0;
}

// initialization constructor, sets coords to x,y,z
Vector3::Vector3(double x, double y, double z)
{
  m[0] = x;
  m[1] = y;
  m[2] = z;
}

// empty default destructor
Vector3::~Vector3()
{
}

// sets the vectors coordinates
void Vector3::set(double x, double y, double z)
{
  m[0] = x;
  m[1] = y;
  m[2] = z;
}

// sets only one of the vectors coordinates
void Vector3::setCoordinate(double value, int coord)
{
  m[coord] = value;
}

// sets vector coordinates to match given vector
void Vector3::setVector(Vector3& other)
{
  m[0] = other.get(0);
  m[1] = other.get(1);
  m[2] = other.get(2);
}

// sets vector coordinates to match given vector
void Vector3::setVector(Vector4& other)
{
  m[0] = other.get(0);
  m[1] = other.get(1);
  m[2] = other.get(2);
}

// gets a particular value of a coordinate
double Vector3::get(int i)
{
  if (i > 2 || i < 0)
  {
    return 0;
  }
  else{
    return m[i];
  }
}

// adds other vector to this vector
Vector3 Vector3::add( Vector3& other)
{
  Vector3 result(0, 0, 0);
  for (int i = 0; i < rows; i++)
  {
    result.setCoordinate(get(i) + other.get(i), i);
  }

  return result;
}

// adds other vector to this vector
void Vector3::addTo(Vector3& other)
{
  for (int i = 0; i < rows; i++)
  {
    setCoordinate(get(i) + other.get(i), i);
  }
}

// subtracts the other vector from this vector
Vector3 Vector3::sub( Vector3& other)
{
  Vector3 result(0, 0, 0);
  for (int i = 0; i < rows; i++)
  {
    result.setCoordinate(get(i) - other.get(i), i);
  }

  return result;
}

// subtracts the other vector from this vector
void Vector3::subBy(Vector3& other)
{
  for (int i = 0; i < rows; i++)
  {
    setCoordinate(get(i) - other.get(i), i);
  }
}

// negates the vector
void Vector3::negate()
{
  for (int i = 0; i < rows; i++)
  {
    setCoordinate(get(i) * -1, i);
  }
}

// scales the vector by s
void Vector3::scale(double s)
{
  for (int i = 0; i < rows; i++)
  {
    setCoordinate(get(i) * s, i);
  }
}

// computes the magnitude of the vector
double Vector3::magnitude()
{
  return sqrt( pow(get(0),2) + pow(get(1),2) + pow(get(2),2) );
}

// computes the dot product of this vector and other
double Vector3::dot(Vector3& other)
{
  double result = 0;
  for (int i = 0; i < rows; i++)
  {
    result += get(i) * other.get(i);
  }

  return result;
}

//computes the angle between the vectors
double Vector3::angle(Vector3& other)
{
	double mag1, mag2;

	mag1 = magnitude();
	mag2 = other.magnitude();

	return acos(dot(other) / (mag1 * mag2));
	
}

// computes cross product of this vector and other vector
Vector3 Vector3::cross(Vector3& other)
{
  Vector3 result(0,0,0);

  // compute x
  double x = get(1) * other.get(2) - get(2) * other.get(1);
  result.setCoordinate(x, 0);

  // compute y
  double y = get(2) * other.get(0) - get(0) * other.get(2);
  result.setCoordinate(y, 1);

  // compute z
  double z = get(0) * other.get(1) - get(1) * other.get(0);
  result.setCoordinate(z, 2);

  return result;
}

// normalizes the vector (create the unit vector)
void Vector3::normalize()
{
  double mag = magnitude();
  for (int i = 0; i < rows; i++)
  {
    setCoordinate(get(i) / mag, i);
  }
}

float* Vector3::getPtrf()
{
	float* result = new float[3];
	result[0] = m[0];
	result[1] = m[1];
	result[2] = m[2];

	return result;
}

// prints values of the vector to standard output
void Vector3::print()
{
  cout << "Vector3:" << endl;
  for (int i = 0; i < rows; i++)
  {
    cout << " | " << get(i) << " |" << endl;
  }
}