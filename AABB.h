#ifndef AABB_H
#define AABB_H

#include "Vector3D.h"
#include <limits>

class AABB {
public:

  /* constructor */
  AABB();
  AABB(float aabb_x, float aabb_y, float aabb_z);

  /* member access function */
  Vector3D getMin();
  Vector3D getMax();

	void addPoint(Vector3D pt);
  void reset();
  Vector3D getSize();
private:
  Vector3D min, max;
};

#endif
