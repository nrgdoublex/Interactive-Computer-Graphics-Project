#include <iostream>
#include <exception>
#include <limits>
#include "Vector3D.h"
#include <cassert>

#define ERROR (0.000001)

/* This file is to test Vector3D class */
int main(int argc, char **argv)
{
  /* Output and Constructor(part a,m) */
  cout << "Test constructor:" << endl;
  Vector3D v1 = Vector3D(0.123456789f,2000000000.2f,-3000.5f);
  Vector3D v2 = Vector3D();
  cout << v1 << "\n" << v2 << endl << endl;

  /* Set value */
  cout << "Test setting value:" << endl;
  v1.Set(Vector3D(3,4,5));
  assert(fabs(v1[0] - 3) < ERROR);
  assert(fabs(v1[1] - 4) < ERROR);
  assert(fabs(v1[2] - 5) < ERROR);
  v1.Set(Vector3D(3.00000004f,-0.987654321f,1000000000.0f));
  cout << v1 << endl;
  assert(fabs(v1[0] - 3.00000004f) < ERROR );
  assert(fabs(v1[1] - (-0.987654321f))< ERROR);
  assert(fabs(v1[2] - 1000000000.0f) < ERROR);
  cout << "Set operator passed!" << endl << endl;

  /* indexing operator(part b) */
  cout << "Test indexing operator:" << endl;
  try{
    cout << "(idx = -1) => " << v1[-1] << endl;
  }
  catch (const char* n){
    cout << "Exception Occurred!!" << endl;
  }
  try{
    cout << "(idx = 100) => " << v1[100] << endl;
  }
  catch (const char* n){
    cout << "Exception Occurred!!" << endl;
  }
  cout << "Index operator passed!" << endl << endl;

  /* vector addition and subtraction(part c,d) */
  cout << "Test addition and subtraction:" << endl;
  int max_float = numeric_limits<float>::max();
  int min_float = numeric_limits<float>::min();
  Vector3D v31(1.0f,1.0f,1.0f);
  Vector3D v3 = v31+Vector3D(3.55436f,-354376.0f,-35.52353450349f);
  assert(fabs(v3[0]-4.55436f) < ERROR);
  assert(fabs(v3[1]+354375.0f) < ERROR);
  assert(fabs(v3[2]+34.52353450349f) < ERROR);
  cout << "Addition and subtraction passed!" << endl << endl;

  /* Dot product(part e) */
  cout << "Test dot product:" << endl;
  Vector3D v5 = Vector3D(1.0f,3.0f,5.0f);
  float v6 = v5*Vector3D(94.8f,3.0f,-1.9f);
  assert(fabs(v6-94.3f) < ERROR);
  cout << "Dot product passed!" << endl << endl;

  /* Cross product(part f) */
  cout << "Test cross product:" << endl;
  Vector3D v7 = Vector3D(1.0f,3.0f,5.0f);
  Vector3D v8 = Vector3D(-2.7f,3.1f,8.5f);
  Vector3D v9 = v7.CrossProduct(v8);
  assert(fabs(v9[0]-10.0f) < ERROR);
  assert(fabs(v9[1]+22.0f) < ERROR);
  assert(fabs(v9[2]-11.2f) < ERROR);
  cout << "Cross product passed!" << endl << endl;

  /* scalar multiplication and division(part g,h) */
  cout << "Test scalar multiplication and division:" << endl;
  Vector3D v10 = Vector3D(10.0f,30.0f,50.0f);
  Vector3D v11 = v10*5;
  assert(fabs(v11[0]-50.0f) < ERROR);
  assert(fabs(v11[1]-150.0f) < ERROR);
  assert(fabs(v11[2]-250.0f) < ERROR);
  try{
    Vector3D v12 = v10/10;
    assert(fabs(v12[0]-1.0f) < ERROR);
    assert(fabs(v12[1]-3.0f) < ERROR);
    assert(fabs(v12[2]-5.0f) < ERROR);
  }
  catch (const char* n){
    cout << "Vector divided by 0!!" << endl;
  }
  cout << "Scalar multiplication and division passed!" << endl << endl;

  /* normalization(part i) */
  cout << "Test normalization:" << endl;
  Vector3D v13 = Vector3D(5.0f,-8.0f,4.0f);
  Vector3D v14 = v13.Normalize();
  assert(fabs(v14[0]-0.48795f) < ERROR);
  assert(fabs(v14[1]+0.78072f) < ERROR);
  assert(fabs(v14[2]-0.39036f) < ERROR);
  cout << "Normalization passed!" << endl << endl;

  /* length(part j) */
  cout << "Test vector length:" << endl;
  assert(fabs(v13.Length() - 10.24695f) < ERROR);
  cout << "Vector length passed!" << endl << endl;

  /* length(part k) */
  cout << "Test rotation to arbitrary axis:" << endl;
  Vector3D v16(1,0,0);
  v16.RotateArbitraryAxis(Vector3D(-1.0f,0.0f,0.0f), Vector3D(0.0f,0.0f,1.0f), 90.0f);
  assert(fabs(v16[0]+1.0f) < ERROR);
  assert(fabs(v16[1]-2.0f) < ERROR);
  assert(fabs(v16[2]-0.0f) < ERROR);
  v16.RotateArbitraryAxis(Vector3D(-1.0f,0.0f,0.0f), Vector3D(0.0f,0.0f,1.0f), 180.0f);
  assert(fabs(v16[0]+1.0f) < ERROR);
  assert(fabs(v16[1]+2.0f) < ERROR);
  assert(fabs(v16[2]-0.0f) < ERROR);
  cout << "Rotation to arbitrary axis passed!" << endl << endl;

  /* length(part k) */
  cout << "Test rotation to arbitrary direction:" << endl;
  Vector3D v17(1,0,0);
  v17.RotateArbitraryDirection(Vector3D(0.0f,0.0f,1.0f), 90.0f);
  assert(fabs(v17[0]-0.0f) < ERROR);
  assert(fabs(v17[1]-1.0f) < ERROR);
  assert(fabs(v17[2]-0.0f) < ERROR);
  v17.RotateArbitraryDirection(Vector3D(0.0f,0.0f,1.0f), 180.0f);
  assert(fabs(v17[0]-0.0f) < ERROR);
  assert(fabs(v17[1]+1.0f) < ERROR);
  assert(fabs(v17[2]-0.0f) < ERROR);
  cout << "Rotation to arbitrary direction passed!" << endl << endl;

  /* input(part m) */
  Vector3D v15 = Vector3D();
  cin >> v15;

  return 0;

}
