#ifndef PPC_H
#define PPC_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Vector3D.h"
#include "Matrix3D.h"
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include <FL/gl.h>
#include <FL/glu.h>


class PPC{

public:
  Vector3D a, b, c, C;
  /* Constructor */
  PPC(unsigned int w, unsigned int h, float fofv);            // part a.

  /* helper function */
  Vector3D getVD();

  /* member access function */
  Vector3D getC();
  void setC(Vector3D newC);
  Vector3D geta();
  void seta(Vector3D newa);
  Vector3D getb();
  void setb(Vector3D newb);
  Vector3D getc();
  void setc(Vector3D newc);
  float getWidth();
  float getHeight();

  /* Translation and rotation */
  void translate(Vector3D tv);
  void leftrightTranslate(float steps);                       // part b.
  void updownTranslate(float steps);                          // part b.
  void forwardbackwardTranslate(float steps);                 // part b.
  void pan(float steps);                                      // part c.
  void tilt(float steps);                                     // part c.
  void row(float steps);                                      // part c.

  /* focal */
  void setFocal(float len);                                   // part d.
  float getFocal();

  /* recover 3D point from projected one */
  Vector3D get3DPoint(float u0, float v0, float zlen);

  /* projection */
  int project(Vector3D point, Vector3D &project);             // part e.

  /* input/output */
  int save(const char *filepath);                             // part g.
  int load(const char *filepath);                             // part g.

  /* interpolation */
  void setInterpolation(PPC *ppc0, PPC *ppc1, float frac);    // part f.
  void rePosition(Vector3D newC, Vector3D newvd, Vector3D up);

  void setIntrinsicsHW(float nearZ, float farZ);
	void setExtrinsicsHW();


private:
  unsigned int width, height;
  float flength;
};

#endif
