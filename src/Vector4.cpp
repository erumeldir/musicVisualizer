#include "Vector4.h"
#include <cmath>
#include <iostream>

using namespace std;

// default construct, sets coords to [0,0,0]
Vector4::Vector4()
{
  m[0] = 0;
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;

}

// initialization constructor, sets coords to x,y,z
Vector4::Vector4(double x, double y, double z)
{
  m[0] = x;
  m[1] = y;
  m[2] = z;
  m[3] = 0;  // TODO: Should this be one or zero?
}

// initialization constructor, sets coords to x,y,z
Vector4::Vector4(double x, double y, double z, double w)
{
  m[0] = x;
  m[1] = y;
  m[2] = z;
  m[3] = w;
}

// empty default destructor
Vector4::~Vector4()
{
}

// sets the vectors coordinates
void Vector4::set(double x, double y, double z, double w)
{
  m[0] = x;
  m[1] = y;
  m[2] = z;
  m[3] = w;

}

// sets only one of the vectors coordinates
void Vector4::setCoordinate(double value, int coord)
{
  m[coord] = value;
}

// sets vector coordinates to match given vector
void Vector4::set(Vector4& other)
{
  m[0] = other.get(0);
  m[1] = other.get(1);
  m[2] = other.get(2);
  m[3] = other.get(3);
}

// gets a particular value of a coordinate
double Vector4::get(int i)
{
  if (i > rows-1 || i < 0)
  {
    return 0;
  }
  else{
    return m[i];
  }
}

// adds other vector to this vector
Vector4 Vector4::add(Vector4& other)
{
  Vector4 result(0, 0, 0, 0);
  for (int i = 0; i < rows; i++)
  {
    result.setCoordinate(get(i) + other.get(i), i);
  }

  return result;
}

// adds other vector to this vector
void Vector4::addTo(Vector4& other)
{
  for (int i = 0; i < rows; i++)
  {
    setCoordinate(get(i) + other.get(i), i);
  }
}

// subtracts the other vector from this vector
Vector4 Vector4::sub(Vector4& other)
{
  Vector4 result(0, 0, 0, 0);
  for (int i = 0; i < rows; i++)
  {
    result.setCoordinate(get(i) - other.get(i), i);
  }

  return result;
}

// subtracts the other vector from this vector
void Vector4::subBy(Vector4& other)
{
  for (int i = 0; i < rows; i++)
  {
    setCoordinate(get(i) - other.get(i), i);
  }
}

// negates the vector
void Vector4::negate()
{
  for (int i = 0; i < rows; i++)
  {
    setCoordinate(get(i) * -1, i);
  }
}

// scales the vector by s
void Vector4::scale(double s)
{
  for (int i = 0; i < rows; i++)
  {
    setCoordinate(get(i) * s, i);
  }
}

// computes the magnitude of the vector
double Vector4::magnitude()
{
  return sqrt(pow(get(0), 2) + pow(get(1), 2) + pow(get(2), 2));
}

// divides everything by the fourth component w
void Vector4::dehomogenize()
{
  for (int i = 0; i < rows; i++)
  {
    setCoordinate(get(i) / get(3), i);
  }
}

float* Vector4::getPtrf()
{
	float* result = new float[4];
	result[0] = m[0];
	result[1] = m[1];
	result[2] = m[2];
	result[3] = m[3];

	return result;
}

// prints values of the vector to standard output
void Vector4::print()
{
  cout << "Vector4:" << endl;
  for (int i = 0; i < rows; i++)
  {
    cout << " | " << get(i) << " |" << endl;
  }
}