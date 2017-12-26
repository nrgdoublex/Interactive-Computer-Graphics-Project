#include "FrameBuffer.h"
#include "FrameBufferWrapper.h"

FrameBuffer::FrameBuffer(int u0, int v0,
	unsigned int width, unsigned int height) : Fl_Gl_Window(u0, v0, width, height, 0)
{
  this->width = width;
  this->height = height;
	this->pixel = new unsigned int[width*height];
	this->zBuffer = new float[width*height];

}

FrameBuffer::~FrameBuffer()
{
	delete [] pixel;
	delete [] zBuffer;
}

void FrameBuffer::draw()
{
	//glDrawPixels(this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, this->pixel);
	if (wrapper->renderid < 2)
		wrapper->renderHW(wrapper->renderid);
	else if (wrapper->renderid == 2)
		wrapper->renderTexHW();
	else if (wrapper->renderid == 3)
		wrapper->renderGPU();
		//wrapper->renderHWCubeMap();
	//wrapper->renderTexHW();
}

void FrameBuffer::KeyboardHandle()
{
	float tstep = 1.0f;
	float rstep = 1.0f;
	int key = Fl::event_key();
	switch (key) {
		case FL_Left: {
			PPC *ppc = wrapper->ppc;
			Vector3D newC = ppc->getC();
			newC.RotateArbitraryDirection(ppc->getb(),tstep*5);
			// commit the new values
			ppc->setC(newC);
			ppc->pan(-tstep*5);

			wrapper->renderAll();
			break;
		}
		case FL_Right: {
			PPC *ppc = wrapper->ppc;
			Vector3D newC = ppc->getC();
			newC.RotateArbitraryDirection(ppc->getb(),-tstep*5);
			// commit the new values
			ppc->setC(newC);
			ppc->pan(tstep*5);

			wrapper->renderAll();
			break;
		}
		case FL_Up: {
			PPC *ppc = wrapper->ppc;
			Vector3D newC = ppc->getC();
			newC.RotateArbitraryDirection(ppc->geta(),-tstep*5);
			// commit the new values
			ppc->setC(newC);
			ppc->tilt(-tstep*5);

			wrapper->renderAll();
			break;
		}
		case FL_Down: {
			PPC *ppc = wrapper->ppc;
			Vector3D newC = ppc->getC();
			newC.RotateArbitraryDirection(ppc->geta(),tstep*5);
			// commit the new values
			ppc->setC(newC);
			ppc->tilt(tstep*5);

			wrapper->renderAll();
			break;
		}
		case 'w': {
			PPC *ppc = wrapper->ppc;
			Vector3D newC = ppc->getC();
			newC.RotateArbitraryDirection(ppc->geta(),-tstep*5);
			// commit the new values
			ppc->setC(newC);
			ppc->tilt(-tstep*5);

			wrapper->renderAll();
			break;
		}
		case 's': {
			PPC *ppc = wrapper->ppc;
			Vector3D newC = ppc->getC();
			newC.RotateArbitraryDirection(ppc->geta(),tstep*5);
			// commit the new values
			ppc->setC(newC);
			ppc->tilt(tstep*5);

			wrapper->renderAll();
			break;
		}
		case 'a': {
			PPC *ppc = wrapper->ppc;
			Vector3D newC = ppc->getC();
			newC.RotateArbitraryDirection(ppc->getb(),tstep*5);
			// commit the new values
			ppc->setC(newC);
			ppc->pan(-tstep*5);

			wrapper->renderAll();
			break;
		}
		case 'd': {
			PPC *ppc = wrapper->ppc;
			Vector3D newC = ppc->getC();
			newC.RotateArbitraryDirection(ppc->getb(),-tstep*5);
			// commit the new values
			ppc->setC(newC);
			ppc->pan(tstep*5);

			wrapper->renderAll();
			break;
		}
		case 'q': {
			wrapper->ppc->row(tstep*5);
			wrapper->renderAll();
			break;
		}
		case 'e': {
			wrapper->ppc->row(-tstep*5);
			wrapper->renderAll();
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

unsigned int FrameBuffer::getColorFromVector(Vector3D vc)
{
	unsigned int ret = 0xFF000000;
	unsigned char *rgba = (unsigned char*)&ret;
	for (int i = 0; i < 3; i++) {
		int ichan = (int)(255.0f*vc[i]);
		ichan = (ichan < 0) ? 0 : ichan;
		ichan = (ichan > 255) ? 255 : ichan;
		rgba[i] = ichan;
	}

	return ret;
}

void FrameBuffer::setBGR(unsigned int bgr)
{
	for (int uv = 0; uv < this->width*this->height; uv++)
		this->pixel[uv] = bgr;
}

void FrameBuffer::set(unsigned int x, unsigned int y, unsigned int color) {
	this->pixel[(this->height - 1 - y)*this->width + x] = color;
}

unsigned int FrameBuffer::get(unsigned int x, unsigned int y) {
	return this->pixel[(this->height - 1 - y)*this->width + x];
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
			if ((cpc - cc).Length() < r)
				setzBuffer(cpc, color);
		}
	}
}

void FrameBuffer::drawCircle(Vector3D c, float r, Vector3D cv) {

	int left = (int)(c[0] - r);
	int right = (int)(c[0] + r);
	int top = (int)(c[1] - r);
	int bottom = (int)(c[1] + r);
	unsigned int color = 0xFF0000FF;
	for (int v = top; v <= bottom; v++) {
		for (int u = left; u <= right; u++) {
			Vector3D currp(.5f+(float)u, .5f + (float) v, c[2]);
			if ((currp - c).Length() < r) {
				setzBuffer(currp, color);
			}
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

void FrameBuffer::draw2DSegment(Vector3D p0, Vector3D p1, Vector3D c0, Vector3D c1)
{

	float du = fabsf(p0[0] - p1[0]);
	float dv = fabsf(p0[1] - p1[1]);
	int stepsN;
	if (du > dv) {
		stepsN = (int)(du + 2);
	}
	else {
		stepsN = (int)(dv + 2);
	}

	Vector3D duv = (p1 - p0) / (float)stepsN;
	Vector3D dc = (c1 - c0) / (float)stepsN;
	for (int stepi = 0; stepi < stepsN; stepi++) {
		Vector3D currp = p0 + duv * (float) stepi;
		Vector3D currc = c0 + dc * (float) stepi;
		this->setzBuffer(currp, getColorFromVector(currc));
	}

}

void FrameBuffer::draw3DSegment(Vector3D p0, Vector3D p1, Vector3D c0, Vector3D c1, PPC *ppc)
{

	Vector3D pp0, pp1;
	if (!ppc->project(p0, pp0))
		return;
	if (!ppc->project(p1, pp1))
		return;
	this->draw2DSegment(pp0, pp1, c0, c1);
}

void FrameBuffer::setzBuffer(Vector3D pt, unsigned int color)
{
	int u = (int)pt[0];
	int v = (int)pt[1];
	if (u < 0 || v < 0 || u > this->width - 1 || v > this->height - 1)
		return;
	int uv = (this->height - 1 - v)*this->width + u;
	if (this->zBuffer[uv] > pt[2])
		return;
	this->zBuffer[uv] = pt[2];

	set(pt[0],pt[1], color);
}

void FrameBuffer::clear(unsigned int bgr, float z0)
{
	setBGR(bgr);
	for (int i=0;i<this->width*this->height;i++)
		this->zBuffer[i] = z0;
}

void FrameBuffer::visualizePPC(PPC *targetPPC, PPC *visualPPC, float flen)
{
	float ptSize = 7.0f;

	Vector3D red(1.0f, 0.0f, 0.0f);
	Vector3D black(0.0f, 0.0f, 0.0f);
	Vector3D center = targetPPC->getC();
	this->draw3DPoint(center, red, ptSize, visualPPC);
	float scf = flen / targetPPC->getFocal();

	this->draw3DSegment(center, center + targetPPC->getc()*scf, red, black, visualPPC);

	Vector3D cs[4];
	float width = targetPPC->getWidth();
	float height = targetPPC->getHeight();
	cs[0] = targetPPC->get3DPoint(0.0f, 0.0f, flen);
	cs[1] = targetPPC->get3DPoint((float)width, 0.0f, flen);
	cs[2] = targetPPC->get3DPoint((float)width, (float)height, flen);
	cs[3] = targetPPC->get3DPoint(0.0f, (float)height, flen);

	for (int i = 0; i < 4; i++) {
		int _i = (i + 1) % 4;
		this->draw3DSegment(cs[i], cs[_i], black, black, visualPPC);
	}
}

void FrameBuffer::draw3DPoint(Vector3D p, Vector3D c, float psize, PPC *ppc) {

	Vector3D pp;
	if (!ppc->project(p, pp))
		return;
	drawCircle(pp, psize, c);
}
