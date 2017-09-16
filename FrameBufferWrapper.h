#pragma once

#include "gui.h"
#include "FrameBuffer.h"
#include <sstream>
#include <iostream>
#include <tiffio.h>
#include <tiffio.hxx>
#include <cstdio>
#include <cstring>
#include <unistd.h>

class FrameBufferWrapper {
public:

	MainUI *gui;
	FrameBuffer *fb;
	FrameBufferWrapper(unsigned int width, unsigned int height);
  void Render();
	void resize(unsigned int width, unsigned int height);
	void setbgr(unsigned int bgr);
	unsigned int getx();
	unsigned int gety();
	unsigned int getwidth();
	unsigned int getheight();
	void set(unsigned int x, unsigned int y, unsigned int color);
	void drawRectangle(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color);
	void drawCircle(unsigned int xc, unsigned int yc, float radius, unsigned int color);
	void drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, unsigned int color);
	int saveImage(const char *filepath);
	int readImage(const char *filepath);
	void animation(unsigned int color);
private:
	unsigned int u0;
	unsigned int v0;
};

extern FrameBufferWrapper *wrapper;
