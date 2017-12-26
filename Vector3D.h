#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>

#define PI (3.1415926f)

using namespace std;

class Vector3D{
public:
  float xyz[3];

  Vector3D();
  Vector3D(float x, float y, float z);              //(a.)
  float& operator[](int idx);                       //(b.)
  Vector3D operator+(Vector3D another);      //(c.)
  Vector3D operator-(Vector3D another);      //(d.)
  float operator*(Vector3D another);        //(e.)
  Vector3D CrossProduct(Vector3D another);   //(f.)
  Vector3D operator*(float operand);                //(g.)
  Vector3D operator/(float operand);                //(h.)
  Vector3D Normalize();                             //(i.)
  float Length();                                   //(j.)
  void RotateArbitraryAxis(Vector3D origin, Vector3D axis, float degree);  //(k.)
  void RotateArbitraryDirection(Vector3D axis, float degree);           //(l.)
  friend ostream& operator<<(ostream &os, Vector3D& self);      //(m.)
  friend istream& operator>>(istream &is, Vector3D& self);     //(m.)


  /* auxiliary functions */
  Vector3D& Set(Vector3D another);
private:

};

#endif
