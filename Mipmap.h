#ifndef MIPMAP_H
#define MIPMAP_H

#include "Vector3D.h"
#include "Texture.h"
#include <cmath>
#include <tiffio.h>
#include <tiffio.hxx>

using namespace std;

class Mipmap{

public:
  Mipmap(const char *filepaths[], unsigned int num_seqs);
  ~Mipmap();
  int readImage2Texture(const char *filepath, Texture *texture);
  unsigned int getPixel(float w, float h, unsigned int dimension);
private:
  Texture *seqs;
  unsigned int *seqs_d;
  unsigned int num_seqs;
};

#endif
