#include "Matrix3D.h"

Matrix3D::Matrix3D()
{

  for (int i=0;i<3;i++){
    rows[i] = Vector3D();
  }
}

Matrix3D::Matrix3D(Vector3D v1, Vector3D v2, Vector3D v3)
{
  rows[0] = Vector3D(v1[0],v1[1],v1[2]);
  rows[1] = Vector3D(v2[0],v2[1],v2[2]);
  rows[2] = Vector3D(v3[0],v3[1],v3[2]);
}

/* On Progress */
Matrix3D::Matrix3D(int axis, float degree)
{
  switch (axis){
    case X_AXIS:
      rows[0] = Vector3D(1,0,0);
      rows[1] = Vector3D(0,cos(degree*PI/180.0),(-1)*sin(degree*PI/180.0));
      rows[2] = Vector3D(0,sin(degree*PI/180.0),cos(degree*PI/180.0));
      break;
    case Y_AXIS:
      rows[0] = Vector3D(cos(degree*PI/180.0),0,sin(degree*PI/180.0));
      rows[1] = Vector3D(0,1,0);
      rows[2] = Vector3D((-1)*sin(degree*PI/180.0),0,cos(degree*PI/180.0));
      break;
    case Z_AXIS:
      rows[0] = Vector3D(cos(degree*PI/180.0),(-1)*sin(degree*PI/180.0),0);
      rows[1] = Vector3D(sin(degree*PI/180.0),cos(degree*PI/180.0),0);
      rows[2] = Vector3D(0,0,1);
      break;
    default:
      cout << "Invalid Axis!" << endl;
  }
  return;
}

Vector3D& Matrix3D::operator[](int idx)
{
  if (idx < 0 || idx > 2)
    throw "Invalid Index!!";
  return this->rows[idx];
}

Vector3D Matrix3D::GetColumn(int idx)
{
  if (idx < 0 || idx > 2)
    throw "Invalid Index!!";
  Matrix3D& self = *this;
  return Vector3D(self[0][idx],self[1][idx],self[2][idx]);
}

Matrix3D& Matrix3D::SetColumn(Vector3D column, int idx)
{
  if (idx < 0 || idx > 2)
    throw "Invalid Index!!";
  Matrix3D& self = *this;
  self[0][idx] = column[0];
  self[1][idx] = column[1];
  self[2][idx] = column[2];
  return *this;
}

Vector3D Matrix3D::operator*(Vector3D v)
{
  Matrix3D& self = *this;
  return Vector3D(self[0]*v,self[1]*v,self[2]*v);
}

Matrix3D Matrix3D::operator*(Matrix3D m)
{
  Matrix3D& self = *this;
  Matrix3D ret;
  for (int i=0;i<3;i++){
    Vector3D col = self*m.GetColumn(i);
    ret.SetColumn(col,i);
  }
  return ret;
}

Matrix3D Matrix3D::Inverse()
{
  Matrix3D& self = *this;
  float determinant = self[0][0] * (self[1][1]*self[2][2]-self[1][2]*self[2][1]) \
                    - self[0][1] * (self[1][0]*self[2][2]-self[1][2]*self[2][0]) \
                    + self[0][2] * (self[1][0]*self[2][1]-self[1][1]*self[2][0]);
  if (!determinant)
    throw "Divided by 0!";
  float minor_00 = self[1][1] * self[2][2] - self[1][2] * self[2][1];
  float minor_01 = self[0][2] * self[2][1] - self[0][1] * self[2][2];
  float minor_02 = self[0][1] * self[1][2] - self[0][2] * self[1][1];
  float minor_10 = self[1][2] * self[2][0] - self[1][0] * self[2][2];
  float minor_11 = self[0][0] * self[2][2] - self[0][2] * self[2][0];
  float minor_12 = self[0][2] * self[1][0] - self[0][0] * self[1][2];
  float minor_20 = self[1][0] * self[2][1] - self[1][1] * self[2][0];
  float minor_21 = self[0][1] * self[2][0] - self[0][0] * self[2][1];
  float minor_22 = self[0][0] * self[1][1] - self[0][1] * self[1][0];

  Vector3D row0 = Vector3D(minor_00/determinant, minor_01/determinant, minor_02/determinant);
  Vector3D row1 = Vector3D(minor_10/determinant, minor_11/determinant, minor_12/determinant);
  Vector3D row2 = Vector3D(minor_20/determinant, minor_21/determinant, minor_22/determinant);
  return Matrix3D(row0,row1,row2);
}

Matrix3D Matrix3D::Transpose()
{
  Matrix3D& self = *this;
  Vector3D row0 = self.GetColumn(0);
  Vector3D row1 = self.GetColumn(1);
  Vector3D row2 = self.GetColumn(2);
  return Matrix3D(row0,row1,row2);
}

ostream& operator<<(ostream &os, Matrix3D& self)
{
  for (int i=0;i<3;i++){
    Vector3D& row = self[i];
    cout << "[" << row[0] << " " << row[1] << " " << row[2] << "]" << endl;
  }
  return os;
}

/* On progress */
istream& operator>>(istream &is, Matrix3D& self)
{
  char line[255];
  char buf[255];
  char *parse;

  /* First row */
  for (int row=0;row<3;row++){
    cout << "Please enter 3 floats for " << (row+1) << "th row(plz separate by single space)" << endl;
    is.getline(line,sizeof(line));
    parse = strtok (line," ");
    int idx = 0;
    while (parse != NULL){
      self[row][idx] = atof(parse);
      parse = strtok (NULL, " ");
      idx += 1;
    }
  }
  free(parse);
  return is;
}
