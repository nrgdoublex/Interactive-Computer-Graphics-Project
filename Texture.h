#ifndef TEXTURE_H
#define TEXTURE_H

#include "Vector3D.h"
#include <cstring>
#include <climits>
#include <cmath>

#define RED_MASK (0x000000FF)
#define GREEN_MASK (0x0000FF00)
#define BLUE_MASK (0x00FF0000)
#define ALPHA_MASK (0xFF000000)

#define GET_RED(pixel) (pixel & RED_MASK)
#define GET_GREEN(pixel) ((pixel & GREEN_MASK) >> 8)
#define GET_BLUE(pixel) ((pixel & BLUE_MASK) >> 16)
#define GET_ALPHA(pixel) ((pixel & ALPHA_MASK) >> 24)

#define GEN_PIXEL(red,green,blue,alpha) (red | (green << 8) | (blue << 16) | (alpha << 24))

using namespace std;

class Texture{
  friend class FrameBufferWrapper;
public:
  Texture() : width(0), height(0), pixel(0) {};
  Texture(unsigned int w, unsigned int h, unsigned int *buf);
  ~Texture();
  unsigned int getPixel(float w, float h);
  unsigned int getWidth();
  unsigned int getHeight();
  unsigned int* getPixel();
  void set(unsigned int w, unsigned int h, unsigned int *buf);
  void reset();
private:
  unsigned int *pixel;
  unsigned int width, height;
};

#endif
