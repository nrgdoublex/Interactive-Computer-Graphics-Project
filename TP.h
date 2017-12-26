#ifndef TP_H
#define TP_H

#include "Vector3D.h"
#include "PPC.h"
#include <tiffio.h>
#include <tiffio.hxx>

using namespace std;

class TP{
public:
  float *zbuffer;
  unsigned int *color;
  PPC *ppc;
  TP(float fofv, Vector3D C, Vector3D vd, Vector3D up, const char *filepath);
  ~TP();
  void setzBuffer(Vector3D pt);
  void clearzBuffer();
  unsigned int renderOrNot(Vector3D pt);
private:
};

#endif
