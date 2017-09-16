#include "Vector3D.h"

Vector3D::Vector3D()
{
  this->xyz[0] = 0.0f;
  this->xyz[1] = 0.0f;
  this->xyz[2] = 0.0f;
}

Vector3D::Vector3D(float x, float y, float z)
{
  this->xyz[0] = x;
  this->xyz[1] = y;
  this->xyz[2] = z;
}

float& Vector3D::operator[](int idx)
{
  if (idx < 0 || idx > 2)
    throw "Invalid Index!!";
  return xyz[idx];
}

Vector3D Vector3D::operator+(Vector3D another)
{
  Vector3D ret;
  ret[0] = this->xyz[0] + another[0];
  ret[1] = this->xyz[1] + another[1];
  ret[2] = this->xyz[2] + another[2];
  return ret;
}

Vector3D Vector3D::operator-(Vector3D another)
{
  Vector3D ret;
  ret[0] = this->xyz[0] - another[0];
  ret[1] = this->xyz[1] - another[1];
  ret[2] = this->xyz[2] - another[2];
  return ret;
}

float Vector3D::operator*(Vector3D another)
{
  Vector3D &self = *this;
  return self[0]*another[0]+self[1]*another[1]+self[2]*another[2];
}

Vector3D Vector3D::CrossProduct(Vector3D another)
{
  Vector3D ret;
  Vector3D &self = *this;
  ret[0] = self[1]*another[2] - self[2]*another[1];
  ret[1] = self[2]*another[0] - self[0]*another[2];
  ret[2] = self[0]*another[1] - self[1]*another[0];
  return ret;
}

Vector3D Vector3D::operator*(float operand)
{
  Vector3D& self = *this;
  Vector3D ret;
  ret[0] = self[0]*operand;
  ret[1] = self[1]*operand;
  ret[2] = self[2]*operand;
  return ret;
}

Vector3D Vector3D::operator/(float operand)
{
  if (!operand){
    throw "Devision by 0!!";
  }

  Vector3D& self = *this;
  Vector3D ret;
  ret[0] = self[0]/operand;
  ret[1] = self[1]/operand;
  ret[2] = self[2]/operand;
  return ret;
}

Vector3D Vector3D::Normalize()
{
  Vector3D& self = *this;
  Vector3D ret;
  float len = self.Length();
  ret[0] = self[0]/len;
  ret[1] = self[1]/len;
  ret[2] = self[2]/len;
  return ret;
}

float Vector3D::Length()
{
  Vector3D& self = *this;
  float square_sum = self[0]*self[0]+self[1]*self[1]+self[2]*self[2];
  return sqrt(square_sum);
}

ostream& operator<<(ostream &os, Vector3D& self)
{
  os << "(" << self[0] << "," << self[1] << "," << self[2] << ")";
  return os;
}

istream& operator>>(istream &is, Vector3D& self)
{
  char line[255];
  char buf[255];
  char *parse;
  cout << "Please enter 3 floats(plz separate by single space)" << endl;
  is.getline(line,sizeof(line));

  /* Parse String */
  parse = strtok (line," ");
  int idx = 0;
  while (parse != NULL)
  {
    self[idx] = atof(parse);
    parse = strtok (NULL, " ");
    idx += 1;
  }
  cout << self << endl;
  return is;
}

Vector3D& Vector3D::Set(Vector3D another)
{
  Vector3D& self = *this;
  self[0] = another[0];
  self[1] = another[1];
  self[2] = another[2];
  return self;
}

void Vector3D::RotateArbitraryAxis(Vector3D origin, Vector3D axis, float degree)
{
  /* create new coordinate system */
  Vector3D x_axis(1,0,0);
  Vector3D y_axis(0,1,0);
  Vector3D a = axis.Normalize();
  Vector3D b;
  if (fabs(a*x_axis) < fabs(a*y_axis)){
    b.Set(x_axis.CrossProduct(a));
  }
  else{
    b.Set(y_axis.CrossProduct(a));
  }

  b.Set(b.Normalize());
  Vector3D c = a.CrossProduct(b);
  c.Set(c.Normalize());

  /* create transition matrix */
  Vector3D aa(a[0],b[0],c[0]);  // row1
  Vector3D bb(a[1],b[1],c[1]);  // row2
  Vector3D cc(a[2],b[2],c[2]);  // row3

  /* Transform to new coordinate system */
  // inverse of transition matrix
  float determinant = aa[0] * (bb[1]*cc[2]-bb[2]*cc[1]) \
                    - aa[1] * (bb[0]*cc[2]-bb[2]*cc[0]) \
                    + aa[2] * (bb[0]*cc[1]-bb[1]*cc[0]);
  if (!determinant)
    throw "Divided by 0!";
  float minor_00 = bb[1] * cc[2] - bb[2] * cc[1];
  float minor_01 = aa[2] * cc[1] - aa[1] * cc[2];
  float minor_02 = aa[1] * bb[2] - aa[2] * bb[1];
  float minor_10 = bb[2] * cc[0] - bb[0] * cc[2];
  float minor_11 = aa[0] * cc[2] - aa[2] * cc[0];
  float minor_12 = aa[2] * bb[0] - aa[0] * bb[2];
  float minor_20 = bb[0] * cc[1] - bb[1] * cc[0];
  float minor_21 = aa[1] * cc[0] - aa[0] * cc[1];
  float minor_22 = aa[0] * bb[1] - aa[1] * bb[0];

  /* coordinate in new system */
  Vector3D new_this = Vector3D( Vector3D(minor_00/determinant, minor_01/determinant, minor_02/determinant)*(*this-origin), \
                              Vector3D(minor_10/determinant, minor_11/determinant, minor_12/determinant)*(*this-origin), \
                              Vector3D(minor_20/determinant, minor_21/determinant, minor_22/determinant)*(*this-origin));
  /* rotation */
  Vector3D afterrotate = Vector3D(Vector3D(1,0,0)*new_this, \
                    Vector3D(0,cos(degree*PI/180.0),(-1)*sin(degree*PI/180.0))*new_this, \
                    Vector3D(0,sin(degree*PI/180.0),cos(degree*PI/180.0))*new_this);

  /* Transform back to the original coordinate system */
  this->Set(origin + Vector3D(aa*afterrotate,bb*afterrotate,cc*afterrotate));
}

void Vector3D::RotateArbitraryDirection(Vector3D axis, float degree)
{
  this->RotateArbitraryAxis(Vector3D(0.0f,0.0f,0.0f),axis,degree);
}
