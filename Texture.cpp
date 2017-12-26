#include "Texture.h"

Texture::Texture(unsigned int w, unsigned int h, unsigned int *buf)
{
  this->width = w;
  this->height = h;
  this->pixel = new unsigned int[w*h];
  memcpy(this->pixel,buf,w*h*sizeof(unsigned int));
}

Texture::~Texture()
{
  delete [] this->pixel;
}

unsigned int Texture::getWidth()
{
  return this->width;
}

unsigned int Texture::getHeight()
{
  return this->height;
}

unsigned int Texture::getPixel(float w, float h)
{
  w = fmod(w,1.0f);
  h = fmod(h,1.0f);
  // we shift the projected point and T00,T10,T01,T11 simultaneously,
  // their relative distance is still not changed
  float wr = w*((float)this->width) - 0.5f;
  float hr = h*((float)this->height) - 0.5f;
  // distance to T00
  float dx = wr - floor(wr);
  float dy = hr - floor(hr);
  int x0, x1, y0, y1;
  x0 = (int)wr; y0 = (int)hr;
  if (x0 == this->width - 1) x1 = x0;
  else x1 = x0 + 1;
  if (y0 == this->height - 1) y1 = y0;
  else y1 = y0 + 1;

  // pixels of 4 corners
  unsigned int T00 = this->pixel[(this->height - 1 - y0)*this->width + x0];
  unsigned int T01 = this->pixel[(this->height - 1 - y1)*this->width + x0];
  unsigned int T10 = this->pixel[(this->height - 1 - y0)*this->width + x1];
  unsigned int T11 = this->pixel[(this->height - 1 - y1)*this->width + x1];

  float red = ((float)GET_RED(T00))*(1.0f-dx)*(1.0f-dy) + ((float)GET_RED(T10))*(dx)*(1.0f-dy) \
                  + ((float)GET_RED(T01))*(1.0f-dx)*(dy) + ((float)GET_RED(T11))*(dx)*(dy);
  float green = ((float)GET_GREEN(T00))*(1.0f-dx)*(1.0f-dy) + ((float)GET_GREEN(T10))*(dx)*(1.0f-dy) \
                  + ((float)GET_GREEN(T01))*(1.0f-dx)*(dy) + ((float)GET_GREEN(T11))*(dx)*(dy);
  float blue = ((float)GET_BLUE(T00))*(1.0f-dx)*(1.0f-dy) + ((float)GET_BLUE(T10))*(dx)*(1.0f-dy) \
                  + ((float)GET_BLUE(T01))*(1.0f-dx)*(dy) + ((float)GET_BLUE(T11))*(dx)*(dy);
  float alpha = ((float)GET_ALPHA(T00))*(1.0f-dx)*(1.0f-dy) + ((float)GET_ALPHA(T10))*(dx)*(1.0f-dy) \
                  + ((float)GET_ALPHA(T01))*(1.0f-dx)*(dy) + ((float)GET_ALPHA(T11))*(dx)*(dy);
  red = (red > 255.0f)?255.0f:((red < 0.0f)?0.0f:red);
  green = (green > 255.0f)?255.0f:((green < 0.0f)?0.0f:green);
  blue = (blue > 255.0f)?255.0f:((blue < 0.0f)?0.0f:blue);
  alpha = (alpha > 255.0f)?255.0f:((alpha < 0.0f)?0.0f:alpha);

  return GEN_PIXEL((unsigned int)red,(unsigned int)green,(unsigned int)blue,(unsigned int)alpha);
}

void Texture::set(unsigned int w, unsigned int h, unsigned int *buf)
{
  if (this->pixel)
    delete [] this->pixel;
  this->width = w;
  this->height = h;
  this->pixel = new unsigned int[w*h];
  memcpy((void*)this->pixel,(void*)buf,w*h*sizeof(unsigned int));
  return;
}

void Texture::reset()
{
  this->width = this->height = 0;
  delete [] this->pixel;
}

unsigned int* Texture::getPixel()
{
  return this->pixel;
}
