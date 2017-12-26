#include "Mipmap.h"

Mipmap::Mipmap(const char *filepaths[], unsigned int num_seqs)
{
  // reallocate buffer, the dimension of each subimage should decrease w.r.t. index
  this->seqs = new Texture[num_seqs];
  this->seqs_d = new unsigned int[num_seqs];
  this->num_seqs = num_seqs;
  for (int i=0;i<num_seqs;i++){
    this->readImage2Texture(filepaths[i],this->seqs+i);
    this->seqs_d[i] = this->seqs[i].getWidth();
  }
}

Mipmap::~Mipmap()
{
  delete [] seqs;
  delete [] seqs_d;
}

int Mipmap::readImage2Texture(const char *filepath, Texture *texture)
{
	unsigned int width, height;
	TIFF* tif = TIFFOpen(filepath, "r");
	if (tif) {
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
		unsigned int buf[width*height];
		TIFFReadRGBAImage(tif, width, height, buf, 0);
		texture->set(width,height,buf);
	}
	else
		return -1;
	TIFFClose(tif);
	return 0;
}

unsigned int Mipmap::getPixel(float w, float h, unsigned int dimension)
{
  if (!this->seqs)
    throw "No mipmap!";
  int idx = 0;
  while (dimension < this->seqs_d[idx]){
    idx = idx + 1;
  }
  int large_image, small_image;
  if (!idx) large_image = idx;
  else large_image = idx - 1;
  small_image = idx;

  float dx = (float)(dimension - this->seqs_d[small_image]) \
          / (this->seqs_d[large_image]-this->seqs_d[small_image]);
  unsigned int small_pixel = this->seqs[small_image].getPixel(w,h);
  unsigned int large_pixel = this->seqs[large_image].getPixel(w,h);
  float red = ((float)GET_RED(small_pixel))*(1-dx) + ((float)GET_RED(large_pixel))*dx;
  float green = ((float)GET_GREEN(small_pixel))*(1-dx) + ((float)GET_GREEN(large_pixel))*dx;
  float blue = ((float)GET_BLUE(small_pixel))*(1-dx) + ((float)GET_BLUE(large_pixel))*dx;
  float alpha = ((float)GET_ALPHA(small_pixel))*(1-dx) + ((float)GET_ALPHA(large_pixel))*dx;
  red = (red > 255.0f)?255.0f:((red < 0.0f)?0.0f:red);
  green = (green > 255.0f)?255.0f:((green < 0.0f)?0.0f:green);
  blue = (blue > 255.0f)?255.0f:((blue < 0.0f)?0.0f:blue);
  alpha = (alpha > 255.0f)?255.0f:((alpha < 0.0f)?0.0f:alpha);

  return GEN_PIXEL((unsigned int)red,(unsigned int)green,(unsigned int)blue,(unsigned int)alpha);
}
