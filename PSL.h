#ifndef PSL_H
#define PSL_H

#include "Vector3D.h"
#include "PPC.h"

class PSL : public PPC {
public:
  float *zbuffer;
  PSL(unsigned int w, unsigned int h, float fofv, Vector3D C, Vector3D vd, Vector3D up) : PPC(w,h,fofv)
  {
    PPC::setC(C);
    float flen = PPC::getFocal();
  	Vector3D newa = (vd.CrossProduct(up)).Normalize()*PPC::geta().Length();
  	Vector3D newb = (vd.CrossProduct(newa)).Normalize()*PPC::getb().Length();
  	Vector3D newc = (vd*flen - newb * (float)h/2.0f \
        - newa * (float)w/2.0f).Normalize()*PPC::getc().Length();
    PPC::seta(newa); PPC::setb(newb); PPC::setc(newc);

    this->zbuffer = new float[w*h];
    for (int i=0;i<w*h;i++)
      this->zbuffer[i] = 0.0f;
  };
  ~PSL();
  void setzBuffer(Vector3D pt);
  void clearzBuffer();
  int renderOrNot(Vector3D pt);
private:
};

#endif
