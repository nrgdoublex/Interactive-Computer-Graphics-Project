#include "TP.h"

using namespace std;

TP::TP(float fofv, Vector3D C, Vector3D vd, Vector3D up, const char *filepath)
{
  unsigned int width, height;
	TIFF* tif = TIFFOpen(filepath, "r");
	if (tif) {
    // read from tiff
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
		this->color = new unsigned int[width*height];
		TIFFReadRGBAImage(tif, width, height, this->color, 0);

    // initialize zbuffer
    this->zbuffer = new float[width*height];
    for (int i=0;i<width*height;i++){
      this->zbuffer[i] = 0.0f;
    }
    this->ppc = new PPC(width, height, fofv);
    this->ppc->setC(C);
    float flen = this->ppc->getFocal();
    Vector3D newa = (vd.CrossProduct(up)).Normalize()*this->ppc->geta().Length();
    Vector3D newb = (vd.CrossProduct(newa)).Normalize()*this->ppc->getb().Length();
    Vector3D newc = (vd*flen - newb * (float)height/2.0f \
        - newa * (float)width/2.0f).Normalize()*this->ppc->getc().Length();
    this->ppc->seta(newa); this->ppc->setb(newb); this->ppc->setc(newc);
	}
	TIFFClose(tif);
}

TP::~TP()
{
  delete [] this->zbuffer;
  delete [] this->color;
  delete this->ppc;
}

void TP::setzBuffer(Vector3D pt)
{
  Vector3D proj;
  this->ppc->project(pt,proj);
  int u = (int)proj[0];
	int v = (int)proj[1];
  unsigned int w = this->ppc->getWidth();
  unsigned int h = this->ppc->getHeight();
	if (u < 0 || v < 0 || u > w - 1 || v > h - 1)
		return;
	int uv = (h - 1 - v)*w + u;
	if (this->zbuffer[uv] > proj[2])
		return;
	this->zbuffer[uv] = proj[2];
  return;
}

void TP::clearzBuffer()
{
  if (!this->zbuffer)
    return;
  for (int i=0;i<this->ppc->getWidth()*this->ppc->getHeight();i++)
    this->zbuffer[i] = 0.0f;
}

// if zbuffer ok, return the texture, else return 0
unsigned int TP::renderOrNot(Vector3D pt)
{
  Vector3D proj;
  this->ppc->project(pt,proj);
  int u = (int)proj[0];
	int v = (int)proj[1];
  unsigned int w = this->ppc->getWidth();
  unsigned int h = this->ppc->getHeight();
  if (u < 0 || v < 0 || u > w - 1 || v > h - 1)
		return 0;
  int uv = (h - 1 - v)*w + u;
  if (this->zbuffer[uv] < proj[2]+0.01f){
    return this->color[uv];
  }
  else{
    return 0;
  }
}
