#include "FrameBufferWrapper.h"

FrameBufferWrapper *wrapper;

FrameBufferWrapper::FrameBufferWrapper(unsigned int width, unsigned int height)
{
	gui = new MainUI();
	int u0 = 20;
	int v0 = 50;
	int w = width;
	int h = height;

	this->u0 = u0;
	this->v0 = v0;
	fb = new FrameBuffer(u0, v0, w, h);
	fb->label("SW framebuffer");
	fb->show();
	gui->uiwindow->position(fb->getwidth() + 20 + 20, 50);
	gui->show();

	Render();
}

void FrameBufferWrapper::Render() {

	unsigned int bgr = 0xFFFFFFFF;
	fb->setBGR(bgr);
	fb->redraw();
}

void FrameBufferWrapper::resize(unsigned int width, unsigned int height)
{
	if (fb)
		delete fb;
	fb = new FrameBuffer(this->u0, this->v0, width, height);
	fb->label("SW framebuffer");
	fb->show();
	//gui->uiwindow->position(fb->width + 20 + 20, 50);
	//gui->show();
	Render();
}

void FrameBufferWrapper::setbgr(unsigned int bgr)
{
	fb->setBGR(bgr);
	fb->redraw();
}

unsigned int FrameBufferWrapper::getx()
{
	return this->u0;
}
unsigned int FrameBufferWrapper::gety()
{
	return this->v0;
}
unsigned int FrameBufferWrapper::getwidth()
{
	return fb->getwidth();
}
unsigned int FrameBufferWrapper::getheight()
{
	return fb->getheight();
}

void FrameBufferWrapper::set(unsigned int x, unsigned int y, unsigned int color)
{
	fb->setGuarded(x,y,color);
	fb->redraw();
}

void FrameBufferWrapper::drawRectangle(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color)
{
	fb->drawRectangle(x0,y0,x1,y1,color);
	fb->redraw();
}

void FrameBufferWrapper::drawCircle(unsigned int xc, unsigned int yc, float radius, unsigned int color)
{
	fb->drawCircle(xc,yc,radius,color);
	fb->redraw();
}

void FrameBufferWrapper::drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, unsigned int color)
{

	fb->drawTriangle(x0,y0,x1,y1,x2,y2,color);
	fb->redraw();
}

int FrameBufferWrapper::saveImage(const char *filepath)
{
	unsigned int width = fb->getwidth();
	unsigned int height = fb->getheight();
	TIFF *output= TIFFOpen(filepath, "w");
	if (output){
		TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);  			// width
		TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);    	// height
		TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 4);  						 	// number of channels per pixel
		TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);    								// size of channels
		TIFFSetField(output, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);  // set the origin of the image.
		//   Some other essential fields to set that you do not have to understand for now.
		TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
		TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

		unsigned int linesize = 4 * width;
		unsigned int *buf = (unsigned int *)_TIFFmalloc(linesize);
		TIFFSetField(output, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(output, linesize));
		for (unsigned int row=0; row<height; row++){
    	memcpy((void *)buf, (const void *)&(fb->pixel)[(height-row-1)*width], linesize);    // check the index here, and figure out why not using h*linebytes
    	if (TIFFWriteScanline(output, buf, row, 0) == -1){
				TIFFClose(output);
				_TIFFfree(buf);
				return -1;
			}
		}
		_TIFFfree(buf);
	}
	else
		return -1;
	TIFFClose(output);
	return 0;
}

int FrameBufferWrapper::readImage(const char *filepath)
{
	unsigned int width;
	unsigned int height;
	TIFF* tif = TIFFOpen(filepath, "r");
	if (tif) {
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
		this->resize(width,height);
		if (TIFFReadRGBAImage(tif, width, height, fb->pixel, 0))
			fb->redraw();
	}
	else
		return -1;
	TIFFClose(tif);
	return 0;
}


void FrameBufferWrapper::animation(unsigned int color)
{
	unsigned int totalFrame = 360;

	char *buf = (char *)malloc(100*sizeof(char));
	Vector3D pt(420,240,100);
	Vector3D pt1(320,140,100);
	for (int i=0;i<totalFrame;i++){
		//pt.RotateArbitraryAxis(Vector3D(320,240,100),Vector3D(-1.732,0,1), 1.0f);
		pt1.RotateArbitraryAxis(Vector3D(320,240,100),Vector3D(0,1.732,1), 1.0f);
		/*for test*/
		//fb->drawCircle(pt[0],pt[1], 2,color);
		fb->drawCircle(pt1[0],pt1[1], 5,color);
		fb->redraw();
		Fl::check();

		/* final function*/
		sprintf(buf, "image/%03d.tiff",i);
		printf("%s\n",buf);
		saveImage((const char*) buf);
	}
	fb->redraw();
	free(buf);
	return;
}
