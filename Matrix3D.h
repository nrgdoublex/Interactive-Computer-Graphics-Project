#ifndef MATRIX3D_H
#define MATRIX3D_H

#include <iostream>
#include <cmath>
#include "Vector3D.h"

using namespace std;

#define X_AXIS (0)
#define Y_AXIS (1)
#define Z_AXIS (2)

class Matrix3D
{
public:
  Matrix3D();
  Matrix3D(Vector3D v1, Vector3D v2, Vector3D v3);  //(a.)
  Matrix3D(int axis, float degree); //(i)
  Vector3D& operator[](int idx);                    //(b.)
  Vector3D GetColumn(int idx);  //(c.)
  Matrix3D& SetColumn(Vector3D column, int idx); //(d.)
  Vector3D operator*(Vector3D v); //(e.)
  Matrix3D operator*(Matrix3D m); //(f.)
  Matrix3D Inverse(); //(g.)
  Matrix3D Transpose(); //(h.)
  friend ostream& operator<<(ostream &os, Matrix3D& self);      //(j.)
  friend istream& operator>>(istream &is, Matrix3D& self);     //(j.)

private:
  Vector3D rows[3];
};

#endif
