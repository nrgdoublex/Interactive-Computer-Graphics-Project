#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int u0, int v0,
	unsigned int width, unsigned int height) : Fl_Gl_Window(u0, v0, width, height, 0)
{
  this->width = width;
  this->height = height;
	this->pixel = new unsigned int[width*height];
}

FrameBuffer::~FrameBuffer()
{
	delete[] pixel;
}

void FrameBuffer::draw()
{
	glDrawPixels(this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, this->pixel);
}

void FrameBuffer::KeyboardHandle()
{
	float tstep = 3.0f;
	float rstep = 3.0f;
	int key = Fl::event_key();
	switch (key) {
	case FL_Left: {
		cerr << "INFO: pressed left" << endl;
		break;
	}
	case FL_Right: {
		break;
	}
	case 'a': {
		cerr << "INFO: pressed a" << endl;
		break;
	}
	default:
		cerr << "INFO: do not understand keypress" << endl;
	}
}

int FrameBuffer::handle(int event)
{
	switch (event)
	{
	case FL_KEYBOARD: {
		KeyboardHandle();
		return 0;
	}
	default:
		break;
	}
	return 0;
}

unsigned int FrameBuffer::getwidth()
{
	return this->width;
}
unsigned int FrameBuffer::getheight()
{
	return this->height;
}

void FrameBuffer::setBGR(unsigned int bgr)
{
	for (int uv = 0; uv < this->width*this->height; uv++)
		this->pixel[uv] = bgr;
}

void FrameBuffer::set(unsigned int x, unsigned int y, unsigned int color) {
	this->pixel[(this->height - 1 - y)*this->width + x] = color;
}

void FrameBuffer::setGuarded(unsigned int x, unsigned int y, unsigned int color) {
	if (x >= this->width || y >= this->height)
		return;
	set(x, y, color);
}

void FrameBuffer::drawRectangle(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color)
{
	for (unsigned int x = x0; x <= x1; x++) {
		for (unsigned int y = y0; y <= y1; y++) {
			setGuarded(x, y, color);
		}
	}
}

void FrameBuffer::drawCircle(unsigned int xc, unsigned int yc, float r, unsigned int color) {

	int left = (unsigned int)(xc - r);
	if (left < 0) left = 0;
	if (left > this->width - 1) return;
	int right = (unsigned int)(xc + r);
	if (right < 0) return;
	if (right > this->width - 1) right = this->width-1;

	int top = (int)(yc - r);
	if (top < 0) top = 0;
	if (top > this->height - 1) return;
	int bottom = (int)(yc + r);
	if (bottom < 0) return;
	if (bottom > this->height - 1) bottom = this->height - 1;

	Vector3D cc(.5f + (float)xc, .5f + (float)yc, 0.0f);
	for (int v = top; v <= bottom; v++) {
		for (int u = left; u <= right; u++) {
			Vector3D cpc(.5f + (float)u, .5f + (float)v, 0.0f);
			if ((cpc - cc).Length() <= r)
				setGuarded(u, v, color);
		}
	}
}

void FrameBuffer::drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, unsigned int color)
{
	float a[3], b[3], c[3];

	// line segment vectors
	a[2] = y1-y0; a[0] = y2-y1; a[1] = y0-y2;
	b[2] = x0-x1; b[0] = x1-x2; b[1] = x2-x0;
	c[2] = y0*x1-x0*y1; c[0] = y1*x2-x1*y2; c[1] = y2*x0-x2*y0;

	// calculate sidedness
	float side_x2 = x2*a[2]+y2*b[2]+c[2];
	float side_x0 = x0*a[0]+y0*b[0]+c[0];
	float side_x1 = x1*a[1]+y1*b[1]+c[1];
	if (side_x2 < 0){
		a[2] = -a[2]; b[2] = -b[2]; c[2] = -c[2];
	}
	if (side_x0 < 0){
		a[0] = -a[0]; b[0] = -b[0]; c[0] = -c[0];
	}
	if (side_x1 < 0){
		a[1] = -a[1]; b[1] = -b[1]; c[1] = -c[1];
	}
	// bounding box
	float max_x = (x0>x1)?((x0>x2)?x0:x2):((x1>x2)?x1:x2);
	float min_x = (x0<x1)?((x0<x2)?x0:x2):((x1<x2)?x1:x2);
	float max_y = (y0>y1)?((y0>y2)?y0:y2):((y1>y2)?y1:y2);
	float min_y = (y0<y1)?((y0<y2)?y0:y2):((y1<y2)?y1:y2);
	if (max_x > width) max_x = width;
	if (min_x < 0) min_x = 0;
	if (max_y > height) max_y = height;
	if (min_y < 0) min_y = 0;
	int left = (int) (min_x+0.5f), right = (int) (max_x-0.5f);
	int top = (int) (min_y+0.5f), bottom = (int) (max_y-0.5f);


	// set color
	int curr_x, curr_y;
	float currEELS[3], currEE[3];
	currEELS[0] = a[0]*(left+0.5f)+b[0]*(top+0.5f)+c[0];
	currEELS[1] = a[1]*(left+0.5f)+b[1]*(top+0.5f)+c[1];
	currEELS[2] = a[2]*(left+0.5f)+b[2]*(top+0.5f)+c[2];
	for (curr_y = top; curr_y <= bottom; curr_y++){
		currEE[0] = currEELS[0];
		currEE[1] = currEELS[1];
		currEE[2] = currEELS[2];
		for (curr_x = left ; curr_x <= right; curr_x++, currEE[0] += a[0], currEE[1] += a[1], currEE[2] += a[2]){
			if(currEE[0] >= 0 && currEE[1] >= 0 && currEE[2] >= 0)
				setGuarded(curr_x,curr_y,color);
		}
		currEELS[0] += b[0];
		currEELS[1] += b[1];
		currEELS[2] += b[2];
	}
}
