#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <cstdio>
#include <cmath>
#include <tiffio.h>
#include <tiffio.hxx>
#include "Vector3D.h"
#include "Texture.h"

using namespace std;

class CubeMap {

public:
  Texture *texture[6];

  CubeMap(const char *filepath);
  ~CubeMap();
  void xyz2uv(Vector3D pt, int *idx, float *u, float *v);
  unsigned int getPixel(int idx, float u, float v);
private:

};

#endif
