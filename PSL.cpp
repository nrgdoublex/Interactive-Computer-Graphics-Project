#include "PSL.h"

using namespace std;

PSL::~PSL()
{
  delete [] this->zbuffer;

}

void PSL::setzBuffer(Vector3D pt)
{
  int u = (int)pt[0];
	int v = (int)pt[1];
  unsigned int w = PPC::getWidth();
  unsigned int h = PPC::getHeight();
	if (u < 0 || v < 0 || u > w - 1 || v > h - 1)
		return;
	int uv = (h - 1 - v)*w + u;
	if (this->zbuffer[uv] > pt[2])
		return;
	this->zbuffer[uv] = pt[2];
}

void PSL::clearzBuffer()
{
  if (!this->zbuffer)
    return;
  for (int i=0;i<PPC::getWidth()*PPC::getHeight();i++)
    this->zbuffer[i] = 0.0f;
}

int PSL::renderOrNot(Vector3D pt)
{
  Vector3D proj;
  PPC::project(pt,proj);
  int u = (int)proj[0];
	int v = (int)proj[1];
  unsigned int w = PPC::getWidth();
  unsigned int h = PPC::getHeight();
  if (u < 0 || v < 0 || u > w - 1 || v > h - 1)
		return -1;
  int uv = (h - 1 - v)*w + u;
  if (this->zbuffer[uv] > proj[2]+0.01f){
    return 0;
  }
  else{
    return -1;
  }
}
