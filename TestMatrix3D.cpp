#include <iostream>
#include "Matrix3D.h"
#include "Vector3D.h"
#include <cmath>
#include <cassert>

#define ERROR (0.000001)

int main(int argc, char **argv)
{
  /* Output and Constructor(part a,j) */
  cout << "Test constructor:" << endl;
  Matrix3D m1 = Matrix3D();
  Vector3D v1 = Vector3D(1,2,3);
  Vector3D v2 = Vector3D(4,5,6);
  Vector3D v3 = Vector3D(7,8,9);
  Matrix3D m2 = Matrix3D(v1,v2,v3);
  for (int i=0;i<3;i++){
    for (int j=0;j<3;j++){
      assert(fabs(m1[i][j]-0.0f) < ERROR);
    }
  }
  for (int i=0;i<3;i++){
    for (int j=0;j<3;j++){
      float v = (float) 3*i+j+1;
      assert(fabs(m2[i][j]-v) < ERROR);
    }
  }
  cout << "Test constructor passed!" << endl << endl;

  /* Indexing operator(part b) */
  cout << "Test indexing operator:" << endl;
  try{
    cout << "(idx = -1) => " << m2[-1] << endl;
  }
  catch (const char* n){
    cout << "Exception Occurred!!" << endl;
  }
  try{
    cout << "(idx = 100) => " << m2[100] << endl;
  }
  catch (const char* n){
    cout << "Exception Occurred!!" << endl;
  }
  cout << "Indexing operator passed!" << endl << endl;

  /* Get Column(part c) */
  cout << "Test GetColumn:" << endl;
  Matrix3D m3(Vector3D(1,2,3),Vector3D(4,5,6),Vector3D(7,8,9));
  Vector3D v31 = m3.GetColumn(0);
  Vector3D v32 = m3.GetColumn(1);
  Vector3D v33 = m3.GetColumn(2);
  assert(fabs(m3[0][0]-1.0f) < ERROR);
  assert(fabs(m3[0][1]-2.0f) < ERROR);
  assert(fabs(m3[0][2]-3.0f) < ERROR);
  assert(fabs(m3[1][0]-4.0f) < ERROR);
  assert(fabs(m3[1][1]-5.0f) < ERROR);
  assert(fabs(m3[1][2]-6.0f) < ERROR);
  assert(fabs(m3[2][0]-7.0f) < ERROR);
  assert(fabs(m3[2][1]-8.0f) < ERROR);
  assert(fabs(m3[2][2]-9.0f) < ERROR);
  try{
    Vector3D m34 = m3.GetColumn(-1);
  }
  catch (const char *n){
    cout << "Invalid index -1!" << endl;
  }
  try{
    Vector3D m35 = m3.GetColumn(100);
  }
  catch (const char *n){
    cout << "Invalid index 100!" << endl;
  }
  cout << "Get column passed!" << endl << endl;

  /* SetColumn(part d) */
  cout << "Test Set column:" << endl;
  Matrix3D m4;
  m4.SetColumn(Vector3D(10,20,30),0);
  m4.SetColumn(Vector3D(40,50,60),1);
  m4.SetColumn(Vector3D(70,80,90),2);
  assert(fabs(m4[0][0]-10.0f) < ERROR);
  assert(fabs(m4[0][1]-40.0f) < ERROR);
  assert(fabs(m4[0][2]-70.0f) < ERROR);
  assert(fabs(m4[1][0]-20.0f) < ERROR);
  assert(fabs(m4[1][1]-50.0f) < ERROR);
  assert(fabs(m4[1][2]-80.0f) < ERROR);
  assert(fabs(m4[2][0]-30.0f) < ERROR);
  assert(fabs(m4[2][1]-60.0f) < ERROR);
  assert(fabs(m4[2][2]-90.0f) < ERROR);
  cout << "Set column passed!" << endl << endl;


  /* Vector Multiplication(part e) */
  cout << "Test Matrix-Vector Multiplication:" << endl;
  Matrix3D m5(Vector3D(1,5,2),Vector3D(2,-4,-3),Vector3D(3.56,325,9));
  Vector3D v4 = Vector3D(3,2,-1);
  Vector3D v5 = m5*v4;
  assert(fabs(v5[0]-11.0f) < ERROR);
  assert(fabs(v5[1]-1.0f) < ERROR);
  assert(fabs(v5[2]-651.68f) < ERROR);
  cout << "Vector multiplication passed!" << endl << endl;

  /* Matrix Multiplication(part f) */
  cout << "Test Matrix-Matrix Multiplication:" << endl;
  Matrix3D m6 = Matrix3D(Vector3D(2,1,4),Vector3D(5,-8,3),Vector3D(7,0,-1));
  Matrix3D m61 = Matrix3D(Vector3D(3,0,4),Vector3D(-5,3,2),Vector3D(8,-7,2));
  Matrix3D m62 = m6*m61;
  assert(fabs(m62[0][0]-33.0f) < ERROR);
  assert(fabs(m62[0][1]+25.0f) < ERROR);
  assert(fabs(m62[0][2]-18.0f) < ERROR);
  assert(fabs(m62[1][0]-79.0f) < ERROR);
  assert(fabs(m62[1][1]+45.0f) < ERROR);
  assert(fabs(m62[1][2]-10.0f) < ERROR);
  assert(fabs(m62[2][0]-13.0f) < ERROR);
  assert(fabs(m62[2][1]-7.0f) < ERROR);
  assert(fabs(m62[2][2]-26.0f) < ERROR);
  cout << "Matrix-Matrix Multiplication passed!" << endl << endl;

  /* Inverse(part g) */
  cout << "Test Inverse:" << endl;
  Matrix3D m7 = Matrix3D(Vector3D(2,1,4),Vector3D(5,-8,3),Vector3D(7,0,-1));
  Matrix3D m71 = m7.Inverse();
  assert(fabs(m71[0][0]-0.03007519f) < ERROR);
  assert(fabs(m71[0][1]-0.0037594f) < ERROR);
  assert(fabs(m71[0][2]-0.13157895f) < ERROR);
  assert(fabs(m71[1][0]-0.09774436f) < ERROR);
  assert(fabs(m71[1][1]+0.11278195f) < ERROR);
  assert(fabs(m71[1][2]-0.05263158f) < ERROR);
  assert(fabs(m71[2][0]-0.21052632f) < ERROR);
  assert(fabs(m71[2][1]-0.02631579f) < ERROR);
  assert(fabs(m71[2][2]+0.07894737f) < ERROR);
  cout << "Inverse passed!" << endl << endl;

  /* Transpose(part h) */
  cout << "Test Transpose:" << endl;
  Matrix3D m8 = Matrix3D(Vector3D(2,1,4),Vector3D(5,-8,3),Vector3D(7,0,-1));
  Matrix3D m81 = m8.Transpose();
  assert(fabs(m81[0][0]-2.0f) < ERROR);
  assert(fabs(m81[0][1]-5.0f) < ERROR);
  assert(fabs(m81[0][2]-7.0f) < ERROR);
  assert(fabs(m81[1][0]-1.0f) < ERROR);
  assert(fabs(m81[1][1]+8.0f) < ERROR);
  assert(fabs(m81[1][2]-0.0f) < ERROR);
  assert(fabs(m81[2][0]-4.0f) < ERROR);
  assert(fabs(m81[2][1]-3.0f) < ERROR);
  assert(fabs(m81[2][2]+1.0f) < ERROR);
  cout << "Transpose passed!" << endl << endl;


  /* rotation matrix(part i) */
  cout << "Test Rotation:" << endl;
  Matrix3D m9 = Matrix3D(X_AXIS,45.0);
  assert(fabs(m9[0][0]-1.0f) < ERROR);
  assert(fabs(m9[0][1]-0.0f) < ERROR);
  assert(fabs(m9[0][2]-0.0f) < ERROR);
  assert(fabs(m9[1][0]-0.0f) < ERROR);
  assert(fabs(m9[1][1]-0.7071068f) < ERROR);
  assert(fabs(m9[1][2]+0.7071068f) < ERROR);
  assert(fabs(m9[2][0]-0.0f) < ERROR);
  assert(fabs(m9[2][1]-0.7071068f) < ERROR);
  assert(fabs(m9[2][2]-0.7071068f) < ERROR);
  Matrix3D m10 = Matrix3D(Y_AXIS,60.0);
  assert(fabs(m10[0][0]-0.5f) < ERROR);
  assert(fabs(m10[0][1]-0.0f) < ERROR);
  assert(fabs(m10[0][2]-0.866025f) < ERROR);
  assert(fabs(m10[1][0]-0.0f) < ERROR);
  assert(fabs(m10[1][1]-1.0f) < ERROR);
  assert(fabs(m10[1][2]-0.0f) < ERROR);
  assert(fabs(m10[2][0]+0.866025f) < ERROR);
  assert(fabs(m10[2][1]-0.0f) < ERROR);
  assert(fabs(m10[2][2]-0.5f) < ERROR);
  Matrix3D m11 = Matrix3D(Z_AXIS,210.0);
  assert(fabs(m11[0][0]+0.866025f) < ERROR);
  assert(fabs(m11[0][1]-0.5f) < ERROR);
  assert(fabs(m11[0][2]-0.0f) < ERROR);
  assert(fabs(m11[1][0]+0.5f) < ERROR);
  assert(fabs(m11[1][1]+0.866025f) < ERROR);
  assert(fabs(m11[1][2]-0.0f) < ERROR);
  assert(fabs(m11[2][0]-0.0f) < ERROR);
  assert(fabs(m11[2][1]-0.0f) < ERROR);
  assert(fabs(m11[2][2]-1.0f) < ERROR);
  cout << "Rotation matrix passed!" << endl << endl;

  /* Input(part j) */
  Matrix3D m12 = Matrix3D();
  cin >> m12;
  cout << m12 << endl;

  return 0;
}
