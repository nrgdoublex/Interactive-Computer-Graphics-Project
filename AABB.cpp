#include "AABB.h"

/*-----------------------------------------------------
  Default constructor
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D scale_ratio: scale factor
Return:

-----------------------------------------------------*/
AABB::AABB()
{
  float fmin = numeric_limits<float>::max();
  this->min = Vector3D(fmin,fmin,fmin);
  this->max = Vector3D(-fmin,-fmin,-fmin);
}

/*-----------------------------------------------------
  Constructor with bounding box size
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  float aabb_x:   size along x-axis
  float aabb_y:   size along y-axis
  float aabb_z:   size along z-axis
Return:

-----------------------------------------------------*/
AABB::AABB(float aabb_x, float aabb_y, float aabb_z)
{
  if (aabb_x < 0 || aabb_y < 0 || aabb_z < 0)
    return;
  float fmin = numeric_limits<float>::max();
  this->min = Vector3D(0.0f,0.0f,0.0f);
  this->max = Vector3D(aabb_x,aabb_y,aabb_z);
}

/*-----------------------------------------------------
  Getting min boundary of bounding box
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
  Vector3D: min boundary of bounding box
-----------------------------------------------------*/
Vector3D AABB::getMin()
{
  return this->min;
}

/*-----------------------------------------------------
  Getting max boundary of bounding box
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
  Vector3D: max boundary of bounding box
-----------------------------------------------------*/
Vector3D AABB::getMax()
{
  return this->max;
}

/*-----------------------------------------------------
  Entending bounding box
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D pt:  point to add to extend
Return:

-----------------------------------------------------*/
void AABB::addPoint(Vector3D pt)
{
	for (int i = 0; i < 3; i++) {
		if (this->max[i] < pt[i])
			this->max[i] = pt[i];
		if (this->min[i] > pt[i])
			this->min[i] = pt[i];
	}

}

/*-----------------------------------------------------
  Reseting bounding box
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:

-----------------------------------------------------*/
void AABB::reset()
{
  float fmin = numeric_limits<float>::max();
  this->min = Vector3D(fmin,fmin,fmin);
  this->max = Vector3D(-fmin,-fmin,-fmin);
}

/*-----------------------------------------------------
  Getting size of bounding box
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
  Vector3D pt:  size of bounding box
-----------------------------------------------------*/
Vector3D AABB::getSize()
{
  return (this->max - this->min);
}
