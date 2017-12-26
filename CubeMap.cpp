#include "CubeMap.h"

CubeMap::CubeMap(const char *filepath)
{
  /* read tiff */
  unsigned int width;
	unsigned int height;
  unsigned int *buf;
	TIFF* tif = TIFFOpen(filepath, "r");
	if (tif) {
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    buf = new unsigned int[width*height];
		TIFFReadRGBAImage(tif, width, height, buf, 0);

    /* create buffer for each subimage */
    int d = width / 3;
    unsigned int buf1[6][d*d];
    /* store subimage */
    for (int i = 0; i < d; i++){
      for (int j = 0; j < d; j++){
        // positive x
        buf1[0][i*d+j] = buf[(height-1-i-d)*width+j+2*d];
        // negative x
        buf1[1][i*d+j] = buf[(height-1-i-d)*width+j];
        // positive y
        buf1[2][i*d+j] = buf[(height-1-i)*width+j+d];
        // negative y
        buf1[3][i*d+j] = buf[(height-1-i-2*d)*width+j+d];
        // positive z
        buf1[4][i*d+j] = buf[(height-1-i-d)*width+j+d];
        // negative z
        buf1[5][(d-1-i)*d+j] = buf[(height-1-i-3*d)*width+j+d];
      }
    }
    for (int i=0;i<6;i++){
      this->texture[i] = new Texture(d,d,buf1[i]);
    }

    delete [] buf;
	}
	TIFFClose(tif);
}

CubeMap::~CubeMap()
{
  for (int i = 0; i < 6; i++)
    delete this->texture[i];
}

void CubeMap::xyz2uv(Vector3D pt, int *idx, float *u, float *v)
{
  float x = pt[0];
  float y = pt[1];
  float z = pt[2];
  float absX = fabs(x);
  float absY = fabs(y);
  float absZ = fabs(z);

  int isXPositive = x > 0 ? 1 : 0;
  int isYPositive = y > 0 ? 1 : 0;
  int isZPositive = z > 0 ? 1 : 0;

  float maxAxis, uc, vc;

  // POSITIVE X
  if (isXPositive && absX >= absY && absX >= absZ) {
    // u (0 to 1) goes from +z to -z
    // v (0 to 1) goes from -y to +y
    maxAxis = absX;
    uc = -z;
    vc = y;
    *idx = 0;
  }
  // NEGATIVE X
  if (!isXPositive && absX >= absY && absX >= absZ) {
    // u (0 to 1) goes from -z to +z
    // v (0 to 1) goes from -y to +y
    maxAxis = absX;
    uc = z;
    vc = y;
    *idx = 1;
  }
  // POSITIVE Y
  if (isYPositive && absY >= absX && absY >= absZ) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from +z to -z
    maxAxis = absY;
    uc = x;
    vc = -z;
    *idx = 2;
  }
  // NEGATIVE Y
  if (!isYPositive && absY >= absX && absY >= absZ) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from -z to +z
    maxAxis = absY;
    uc = x;
    vc = z;
    *idx = 3;
  }
  // POSITIVE Z
  if (isZPositive && absZ >= absX && absZ >= absY) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from -y to +y
    maxAxis = absZ;
    uc = x;
    vc = y;
    *idx = 4;
  }
  // NEGATIVE Z
  if (!isZPositive && absZ >= absX && absZ >= absY) {
    // u (0 to 1) goes from +x to -x
    // v (0 to 1) goes from -y to +y
    maxAxis = absZ;
    uc = -x;
    vc = y;
    *idx = 5;
  }

  // Convert range from -1 to 1 to 0 to 1
  *u = 0.5f * (uc / maxAxis + 1.0f);
  *v = 0.5f * (vc / maxAxis + 1.0f);
}

unsigned int CubeMap::getPixel(int idx, float u, float v)
{
  return this->texture[idx]->getPixel(u,v);
}
