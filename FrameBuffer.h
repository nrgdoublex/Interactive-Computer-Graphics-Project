#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include "math.h"
#include "Vector3D.h"
#include <iostream>


using namespace std;

class FrameBuffer : public Fl_Gl_Window {
public:
  unsigned int* pixel;
  FrameBuffer(int u0, int v0, unsigned int width, unsigned int height);   // part a.
  ~FrameBuffer();
  void draw();
	void KeyboardHandle();
	int handle(int guievent);
  unsigned int getwidth();
  unsigned int getheight();
  void set(unsigned int x, unsigned int y, unsigned int color); // part c.
  void setBGR(unsigned int bgr);  // part b.
  void setGuarded(unsigned int x, unsigned int y, unsigned int color); // part c.
  void drawRectangle(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color);   // part d.
  void drawCircle(unsigned int xc, unsigned int yc, float r, unsigned int color);   // part f.
  void drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, unsigned int color);    // part e.
private:
  unsigned int width, height;
};
