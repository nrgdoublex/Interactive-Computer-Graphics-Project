#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include "math.h"
#include "Vector3D.h"
#include "PPC.h"

using namespace std;

class FrameBuffer : public Fl_Gl_Window {
public:
  unsigned int* pixel;
  FrameBuffer(int u0, int v0, unsigned int width, unsigned int height);
  ~FrameBuffer();
  void draw();
	void KeyboardHandle();
	int handle(int guievent);
  unsigned int getwidth();
  unsigned int getheight();
  unsigned int getColorFromVector(Vector3D vc);
  void set(unsigned int x, unsigned int y, unsigned int color);
  unsigned int get(unsigned int x, unsigned int y);
  void setBGR(unsigned int bgr);
  void setGuarded(unsigned int x, unsigned int y, unsigned int color);
  void drawRectangle(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color);   // part d.
  void drawCircle(Vector3D c, float r, Vector3D cv);
  void drawCircle(unsigned int xc, unsigned int yc, float r, unsigned int color);
  void drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, unsigned int color);    // part e.
  void draw2DSegment(Vector3D p0, Vector3D p1, Vector3D c0, Vector3D c1);
  void draw3DSegment(Vector3D p0, Vector3D p1, Vector3D c0, Vector3D c1, PPC *ppc);
  void setzBuffer(Vector3D pt, unsigned int color);
  void clear(unsigned int bgr, float z0);
  void visualizePPC(PPC *targetPPC, PPC *visualPPC, float flen);
  void draw3DPoint(Vector3D p, Vector3D c, float psize, PPC *ppc);
  

private:
  unsigned int width, height;
  float *zBuffer;
};

#endif
