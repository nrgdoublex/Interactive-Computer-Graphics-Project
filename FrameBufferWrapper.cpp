#include "FrameBufferWrapper.h"

//#define TRD_PERSON_ENABLED

#ifdef TRD_PERSON_ENABLED
#define TRDPERSON (1)
#else
#define TRDPERSON (0)
#endif

FrameBufferWrapper *wrapper;

/*-----------------------------------------------------
  Constructor with width and height
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  unsigned int width:			width of scene
	unsigned int height:		height of scene
Return:

-----------------------------------------------------*/
FrameBufferWrapper::FrameBufferWrapper(unsigned int width, unsigned int height)
{
	gui = new MainUI();
	int u0 = 20;
	int v0 = 50;
	int w = width;
	int h = height;

	this->u0 = u0;
	this->v0 = v0;
	this->switch_flag = 1;
	this->fb = new FrameBuffer(u0, v0, w, h);
	this->fb->label("First Person");
	this->fb->show();
#ifdef TRD_PERSON_ENABLED
	this->fb3 = new FrameBuffer(u0, v0, w, h);
	this->fb3->label("Third Person");
	this->fb3->position(u0 + fb->getwidth() + 20, v0);
	this->fb3->show();
#endif
	this->gui->uiwindow->position(u0, v0 + this->fb->getheight() + 20);
	this->gui->show();

	/* initialization */
	this->tm = NULL;
	this->t_scale = NULL;
	this->texture = NULL;
	this->mipmap = NULL;
	this->psl = NULL;
	this->tp = NULL;
	this->bbppc = NULL;
	this->billboard = NULL;
	this->cubemap = NULL;
	this->cgi = NULL;
	this->soi = NULL;

	//demoHWFilledMode();
	//renderHWCubeMap();
	demoGPUShader();

	/* test */
	//this->texture = new Texture[1];
	//this->readImage2Texture("./image/palace256.tiff",this->texture);




#ifdef TRD_PERSON_ENABLED
	this->ppc3 = new PPC(width, height, 90.0f);
	Vector3D newC = this->ppc->getC() + Vector3D(50.0f, 100.0f, 50.0f);
	this->ppc3->rePosition(newC, this->tm[0].getCenterOfMass()-newC, Vector3D(0.0f, 1.0f, 0.0f));
#endif


	/* render*/
	renderAll();

}

/*-----------------------------------------------------
  Destructor
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:

-----------------------------------------------------*/
FrameBufferWrapper::~FrameBufferWrapper()
{
	delete [] tm;
	delete ppc;
	delete gui;
	delete fb;
	delete fb3;
	delete ppc3;
	delete [] texture;
	delete [] t_scale;
	delete mipmap;
	delete psl;
	delete tp;
	delete cubemap;
}

/*-----------------------------------------------------
  Underlying function of Debug button
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:

-----------------------------------------------------*/
void FrameBufferWrapper::debug()
{
}

/*-----------------------------------------------------
  Drawing all objects in some framebuffer w.r.t. some camera
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	FrameBuffer *currfb:		framebuffer to draw
	PPC *currppc:						camera w.r.t.
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::render(FrameBuffer *currfb, PPC *currppc)
{
	unsigned int bgr = 0xFFFFFFFF;
	currfb->clear(bgr,0.0f);
	for (int tmi = 0; tmi < this->num_tm; tmi++) {
		if (this->renderid == REFLECTIONMAP)
			this->tm[tmi].RenderReflectionMap(currppc,currfb,this->cubemap);
		else if (this->renderid == REFRACTIONMAP)
			this->tm[tmi].RenderRefractionMap(currppc,currfb,this->cubemap);
		else if (this->renderid == BILLBOARD){
			if (!tmi)
				this->tm[tmi].RenderBillBoard(currppc,currfb, this->bbppc, this->bbfb, this->cubemap);
			else
				this->tm[tmi].RenderReflectionMap(currppc,currfb,this->cubemap);
		}
		else
			this->tm[tmi].RenderFilledIS(currppc,currfb);
	}

	//(currppc, currfb);

	currfb->redraw();
	return;
}

/*-----------------------------------------------------
  Drawing all objects in some framebuffer w.r.t. some camera, for 3rd-person view
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	FrameBuffer *currfb:	framebuffer to draw
	PPC *currppc:					camera w.r.t.
	PPC *targetppc:				camera to render
	int trdperson:				flag
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::render(FrameBuffer *currfb, PPC *currppc, PPC *targetppc, int trdperson)
{

	unsigned int bgr = 0xFFFFFFFF;
	currfb->clear(bgr,0.0f);

	for (int tmi = 0; tmi < this->num_tm; tmi++) {
		this->tm[tmi].RenderPoints(currppc, currfb);
		this->tm[tmi].RenderWireframe(currppc, currfb);
	}

	/* 3rd-person mode */
	if (trdperson){
		float visz = 40.0f;
		currfb->visualizePPC(targetppc, currppc, visz);
	}

	currfb->redraw();
}

/*-----------------------------------------------------
  Wrapper function for render
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:

-----------------------------------------------------*/
void FrameBufferWrapper::renderAll()
{
	 //render(this->fb,this->ppc);

	/* hardware rendering */
	// clear the framebuffer
	// clear the framebuffer
	this->fb->redraw();

#ifdef TRD_PERSON_ENABLED
	render(this->fb3,this->ppc3,this->ppc,TRDPERSON);
#endif
}

/*-----------------------------------------------------
  Resizing framebuffer
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	unsigned int width:			width of new framebuffer
	unsigned int height:		height of new framebuffer
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::resize(unsigned int width, unsigned int height)
{
	if (this->fb)
		delete this->fb;
	this->fb = new FrameBuffer(this->u0, this->v0, width, height);
	this->fb->label("SW framebuffer");
	this->fb->show();
	//gui->uiwindow->position(fb->width + 20 + 20, 50);
	//gui->show();
	renderAll();
}

/*-----------------------------------------------------
  Setting background color
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	unsigned int bgr:		background color
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::setbgr(unsigned int bgr)
{
	fb->setBGR(bgr);
	fb->redraw();
}

/*-----------------------------------------------------
  Getting top-left x-coor. of framebuffer
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
	unsigned int: x-coor of framebuffer
-----------------------------------------------------*/
unsigned int FrameBufferWrapper::getx()
{
	return this->u0;
}
/*-----------------------------------------------------
  Getting top-left y-coor. of framebuffer
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
	unsigned int: y-coor of framebuffer
-----------------------------------------------------*/
unsigned int FrameBufferWrapper::gety()
{
	return this->v0;
}
/*-----------------------------------------------------
  Getting width of framebuffer
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
	unsigned int: width of framebuffer
-----------------------------------------------------*/
unsigned int FrameBufferWrapper::getwidth()
{
	return fb->getwidth();
}
/*-----------------------------------------------------
  Getting height of framebuffer
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
	unsigned int: width of framebuffer
-----------------------------------------------------*/
unsigned int FrameBufferWrapper::getheight()
{
	return fb->getheight();
}

/*-----------------------------------------------------
  Drawing a 2D point
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	unsigned int x:				x-coor. of point
	unsigned int y:				y-coor. of point
	unsigned int color:		color
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::set(unsigned int x, unsigned int y, unsigned int color)
{
	fb->setGuarded(x,y,color);
	fb->redraw();
}
/*-----------------------------------------------------
  Drawing a 2D rectangle
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	unsigned int x0:			x-coor. of top-left point
	unsigned int y0:			y-coor. of top-left point
	unsigned int x1:			x-coor. of bottom-right point
	unsigned int y1:			y-coor. of bottom-right point
	unsigned int color:		color
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::drawRectangle(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color)
{
	fb->drawRectangle(x0,y0,x1,y1,color);
	fb->redraw();
}
/*-----------------------------------------------------
  Drawing a 2D rectangle
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	unsigned int xc:			x-coor. of center
	unsigned int yc:			y-coor. of center
	float radius:					radius of circle
	unsigned int color:		color
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::drawCircle(unsigned int xc, unsigned int yc, float radius, unsigned int color)
{
	fb->drawCircle(xc,yc,radius,color);
	fb->redraw();
}

/*-----------------------------------------------------
  Drawing a 2D rectangle
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	float x0:							x-coor. of 1st point
	float y0:							y-coor. of 1st point
	float x1:							x-coor. of 2nd point
	float y1:							y-coor. of 2nd point
	float x2:							x-coor. of 3rd point
	float y2:							y-coor. of 3rd point
	unsigned int color:		color
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, unsigned int color)
{

	fb->drawTriangle(x0,y0,x1,y1,x2,y2,color);
	fb->redraw();
}
/*-----------------------------------------------------
  Saving framebuffer
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	const char *filepath:  file to save
Return:
	0 for success, -1 otherwise
-----------------------------------------------------*/
int FrameBufferWrapper::saveImage(const char *filepath)
{
	unsigned int width = fb->getwidth();
	unsigned int height = fb->getheight();
	glReadPixels(0.0f,0.0f,width,height,GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid *)this->fb->pixel);
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

/*-----------------------------------------------------
  Loading framebuffer from file
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	const char *filepath:  file to load from
Return:
	0 for success, -1 otherwise
-----------------------------------------------------*/
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

int FrameBufferWrapper::readImage2Texture(const char *filepath, Texture *texture)
{
	unsigned int width, height;
	TIFF* tif = TIFFOpen(filepath, "r");
	if (tif) {
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
		unsigned int buf[width*height];
		TIFFReadRGBAImage(tif, width, height, buf, 0);
		texture->set(width,height,buf);
	}
	else
		return -1;
	TIFFClose(tif);
	return 0;
}

/*-----------------------------------------------------
  For animation
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:

-----------------------------------------------------*/
void FrameBufferWrapper::animation()
{


	unsigned int totalframe = 300;
	char buf[256];

	demoHWFilledMode();
	for (int i=0;i<totalframe;i++){
		Vector3D newC = this->ppc->getC();
		newC.RotateArbitraryDirection(this->ppc->getb(),1.2f);
		// commit the new values
		this->ppc->setC(newC);
		this->ppc->pan(-1.2f);
		renderAll();
		Fl::check();
		//sprintf(buf, "output/%04d.tiff",i);
		//printf("%s\n",buf);
		//saveImage((const char*) buf);
	}

	demoHWWireFrameMode();
	for (int i=0;i<totalframe;i++){
		Vector3D newC = this->ppc->getC();
		newC.RotateArbitraryDirection(this->ppc->getb(),1.2f);
		// commit the new values
		ppc->setC(newC);
		ppc->pan(-1.2f);

		renderAll();
		Fl::check();
		//sprintf(buf, "output/%04d.tiff",i+totalframe);
		//printf("%s\n",buf);
		//saveImage((const char*) buf);
	}

	demoHWTexMap();
	for (int i=0;i<totalframe;i++){
		Vector3D newC = this->ppc->getC();
		newC.RotateArbitraryDirection(this->ppc->getb(),1.2f);
		// commit the new values
		ppc->setC(newC);
		ppc->pan(-1.2f);

		renderAll();
		Fl::check();
		//sprintf(buf, "output/%04d.tiff",i+totalframe*2);
		//printf("%s\n",buf);
		//saveImage((const char*) buf);
	}

	demoGPUShader();
	for (int i=0;i<totalframe;i++){
		Vector3D newC = this->ppc->getC();
		newC.RotateArbitraryDirection(this->ppc->getb(),-1.2f);
		// commit the new values
		ppc->setC(newC);
		ppc->pan(1.2f);

		renderAll();
		Fl::check();
		//sprintf(buf, "output/%04d.tiff",i);
		//printf("%s\n",buf);
		//saveImage((const char*) buf);
	}
	for (int i=0;i<totalframe;i++){
		Vector3D newC = this->ppc->getC();
		newC.RotateArbitraryDirection(this->ppc->geta(),-1.2f);
		// commit the new values
		ppc->setC(newC);
		ppc->tilt(-1.2f);

		renderAll();
		Fl::check();
		//sprintf(buf, "output/%04d.tiff",i+totalframe);
		//printf("%s\n",buf);
		//saveImage((const char*) buf);
	}

	return;
}

/*-----------------------------------------------------
  Wrapper function for setting focal length
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	float flen: 	new focal length
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::setFocal(float flen)
{
	this->ppc->setFocal(flen);
	renderAll();
}

/*-----------------------------------------------------
  Implementation of interpolation between 2 cameras
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:

-----------------------------------------------------*/
void FrameBufferWrapper::interpolation()
{

	PPC ppc0 = *this->ppc;
	Vector3D O = this->tm[0].getCenterOfMass();
	Vector3D newC = this->ppc->getC() + Vector3D(100.0f, 70.0f, 50.0f);
	this->ppc->rePosition(newC, O - newC, Vector3D(0.0f, 1.0f, 0.0f));
	PPC ppc1 = *this->ppc;
	int framesN = 300;
	for (int fi = 0; fi < framesN; fi++) {
		float fracf = (float)fi / (float)(framesN - 1);
		this->ppc->setInterpolation(&ppc0, &ppc1, fracf);
		renderAll();
		Fl::check();
	}
	*this->ppc = ppc0;
	renderAll();
	return;
}

/*-----------------------------------------------------
  Wrapper function for translating triangle mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	float x:		amounts to translate along x-axis
	float y:		amounts to translate along y-axis
	float z:		amounts to translate along z-axis
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::translateTM(float x, float y, float z)
{
	Vector3D shift(x,y,z);
	for (int i=0;i<this->num_tm;i++){
		this->tm[i].translate(shift);
	}
	renderAll();
	return;
}

/*-----------------------------------------------------
  Wrapper function for translating triangle mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	float sfx:		factor to scale along x-axis
	float sfy:		factor to scale along y-axis
	float sfz:		factor to scale along z-axis
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::scaleTM(float sfx, float sfy, float sfz)
{
	if (sfx < 0 || sfy < 0 || sfz < 0)
		return;
	Vector3D sfv(sfx,sfy,sfz);
	for (int i=0;i<this->num_tm;i++){
		this->tm[i].reScale(sfv);
	}
	renderAll();
}

/*-----------------------------------------------------
  Wrapper function for repositon and rescaling
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	float cx:				x-coor. of new centroid
	float cy:				y-coor. of new centroid
	float cz:				z-coor. of new centroid
	float aabb_x:		size of new bounding box along x-axis
	float aabb_y:		size of new bounding box along y-axis
	float aabb_z:		size of new bounding box along z-axis
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::rePositionScaleTM(float cx, float cy, float cz, float aabb_x, float aabb_y, float aabb_z)
{
	if (aabb_x < 0 || aabb_y < 0 || aabb_y < 0)
		return;
	Vector3D sfv(cx,cy,cz);
	AABB bbox = AABB(aabb_x,aabb_y,aabb_z);
	if (this->tm)
		this->tm[0].reScaleAndPosition(sfv,bbox);
	renderAll();
}
/*-----------------------------------------------------
  Switching between filled and wireframe mode
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:

-----------------------------------------------------*/
void FrameBufferWrapper::switchFilledWireframe()
{
	this->switch_flag = (this->switch_flag + 1) % 2;
	renderAll();
}
/*-----------------------------------------------------
  Wrapper function for loading binary bin mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
	const char *filepath: file name
Return:

-----------------------------------------------------*/
void FrameBufferWrapper::loadBin(const char *filepath)
{
	if (!filepath || !this->tm)
		return;

	this->tm[0].LoadBin(filepath);
	this->ppc->setC(this->tm[0].getCenterOfMass() + Vector3D(0.0f, 0.0f, 100.0f));
	renderAll();
}

void FrameBufferWrapper::clearObjects()
{
	if (this->tm){
		delete [] this->tm;
		this->tm = NULL;
	}
	if (this->t_scale){
		delete [] this->t_scale;
		this->t_scale = NULL;
	}
	if (this->texture){
		delete [] this->texture;
		this->texture = NULL;
	}
	if (this->mipmap){
		delete this->mipmap;
		this->mipmap = NULL;
	}
	if (this->psl){
		delete this->psl;
		this->psl = NULL;
	}
	if (this->tp){
		delete this->tp;
		this->tp = NULL;
	}
	if (this->cubemap){
		delete this->cubemap;
		this->cubemap = NULL;
	}
	if (this->bbppc){
		delete this->bbppc;
		this->bbppc = NULL;
	}
	if (this->billboard){
		delete this->billboard;
		this->billboard = NULL;
	}
	if (this->cgi){
		delete this->cgi;
		this->cgi = NULL;
	}
	if (this->soi){
		delete this->soi;
		this->soi = NULL;
	}
}

void FrameBufferWrapper::showTexture(const char *filepath, float scale, unsigned int width, unsigned int height)
{
	if (!filepath)
		return;

	/* clear all objects concerning mesh */
	clearObjects();

	/* reallocate mesh */
	this->num_tm = 1;
	this->tm = new TriangleMesh[this->num_tm];
	this->t_scale = new float[1];
	this->t_scale[0] = scale;
	this->tm[0].SetRectangleQuad(Vector3D(0.0f, 200.0f, -300.0f), Vector3D(200.0f, 200.0f, 200.0f),
		Vector3D(1.0f, 0.5f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f),0.0f);

	/* create texture */
	this->texture = new Texture[1];
	this->readImage2Texture(filepath,this->texture);

	/* reallocate ppc */
	if (this->ppc)
		delete this->ppc;
	this->ppc = new PPC(width, height, 55.0f);
	this->ppc->setC(this->tm[0].getCenterOfMass() + Vector3D(0.0f, 0.0f, 400.0f));

	this->renderTexture(this->fb,this->ppc, this->t_scale);
}

void FrameBufferWrapper::showMipmap(const char *filepaths[], float scale, \
								unsigned int width, unsigned int height, unsigned int num_seqs)
{
	if (!filepaths)
		return;

	/* clear all objects concerning mesh */
	clearObjects();

	/* reallocate mesh */
	this->num_tm = 1;
	this->tm = new TriangleMesh[this->num_tm];
	this->tm[0].SetRectangleQuad(Vector3D(0.0f, 200.0f, -300.0f), Vector3D(64.0f, 64.0f, 64.0f),
		Vector3D(1.0f, 0.5f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f),0.0f);
	this->mipmap = new Mipmap(filepaths,num_seqs);

	/* reallocate ppc */
	if (this->ppc)
		delete this->ppc;
	this->ppc = new PPC(width, height, 55.0f);
	this->ppc->setC(this->tm[0].getCenterOfMass() + Vector3D(0.0f, 0.0f, 400.0f));

	this->renderMipmap(this->fb,this->ppc, scale);
}

void FrameBufferWrapper::renderTexture(FrameBuffer *currfb, PPC *currppc, float *scale)
{
	unsigned int bgr = 0xFFFFFFFF;
	currfb->clear(bgr,0.0f);
	for (int tmi = 0; tmi < this->num_tm; tmi++) {
		this->tm[tmi].RenderTextureQuad(currppc,currfb,this->texture+tmi,scale[tmi]);
	}
	currfb->redraw();
	return;
}

void FrameBufferWrapper::renderMipmap(FrameBuffer *currfb, PPC *currppc, float scale)
{
	unsigned int bgr = 0xFFFFFFFF;
	currfb->clear(bgr,0.0f);
	this->tm[0].RenderTextureQuad(currppc,currfb,this->mipmap,scale);
	currfb->redraw();
	return;
}

void FrameBufferWrapper::modelInterpolation()
{
	clearObjects();
	this->num_tm = 1;
	this->tm = new TriangleMesh[this->num_tm];

	/* set objects */
	char buf[256];
	getcwd(buf,256);
	strcat(buf,"/geometry/teapot1K.bin");
	this->tm[0].LoadBin(buf);
	//this->renderModelInterpolation(this->fb,this->ppc);

	unsigned int width, height;
	if (this->ppc){
		width = this->ppc->getWidth();
		height = this->ppc->getHeight();
		delete this->ppc;
	}
	else{
		width = 640;
		height = 480;
	}
	this->ppc = new PPC(width, height, 55.0f);
	this->ppc->setC(this->tm[0].getCenterOfMass() + Vector3D(0.0f, 0.0f, 100.0f));

	this->renderModelInterpolation(this->fb,this->ppc);
}

void FrameBufferWrapper::renderModelInterpolation(FrameBuffer *currfb, PPC *currppc)
{
	unsigned int bgr = 0xFFFFFFFF;
	currfb->clear(bgr,0.0f);
	for (int tmi = 0; tmi < this->num_tm; tmi++) {
		this->tm[tmi].RenderFilledMS(currppc,currfb,this->psl);
	}
	currfb->redraw();
	return;
}

void FrameBufferWrapper::showShadowMap()
{
	clearObjects();
	this->renderid = SHADOWMAP;
	/* set triangle mesh */
	this->num_tm = 3;
	this->tm = new TriangleMesh[this->num_tm];

	/* set objects */
	char buf[256];
	getcwd(buf,256);
	strcat(buf,"/geometry/teapot1K.bin");
	this->tm[0].SetRectangleQuad(Vector3D(200.0f, 0.0f, -500.0f), Vector3D(1000.0f, 1000.0f, 1000.0f),
		Vector3D(0.7f, 0.7f, 0.7f), Vector3D(0.0f, 1.0f, 0.0f),90.0f);
	this->tm[1].SetBoundingBox(Vector3D(-200.0f, 50.0f, -300.0f),Vector3D(100.0f, 100.0f, 100.0f),
		Vector3D(1.0f, 0.5f, 0.0f));
	this->tm[1].rotationAboutAxis(this->tm[1].getCenter(),Vector3D(0.0f,1.0f,0.0f),-30.0f);
	this->tm[2].LoadBin(buf);
	this->tm[2].translate(Vector3D(0.0f,50.0f,-300.0f));
	this->tm[2].rotationAboutAxis(this->tm[2].getCenter(),Vector3D(0.0f,1.0f,0.0f),-90.0f);
	this->tm[2].reScale(Vector3D(2.0f,2.0f,2.0f));

	/* Shadow map */
	this->psl = new PSL(640,480,120.0f,Vector3D(-600.0f, 50.0f, -400.0f) \
		, Vector3D(1.0f,0.0f,0.0f), Vector3D(0.0f,1.0f,0.0f));
	this->ppc = new PPC(this->getwidth(), this->getheight(), 55.0f);
	this->ppc->setC(this->tm[1].getCenterOfMass() + Vector3D(-400.0f, 100.0f, 500.0f));
	this->ppc->pan(-50.0f);
	this->ppc->updownTranslate(-100.0f);
	this->ppc->tilt(-10.0f);


	int framesN = 300;
	//char buf[256];
	for (int fi = 0; fi < framesN; fi++) {
		float fracf = (float)fi / (float)(framesN - 1);
		this->psl->translate(Vector3D(0.0f,0.0f,2.0f));
		this->psl->clearzBuffer();
		for (int i=1;i<this->num_tm;i++){
			this->tm[i].buildShadowMap(this->psl);
		}
		renderAll();
		Fl::check();

		/* for video */
		/*sprintf(buf, "image/shadow%03d.tiff",fi);
		printf("%s\n",buf);
		saveImage((const char*) buf);*/
	}
	renderAll();
}

void FrameBufferWrapper::showProjTexMap()
{
	clearObjects();
	this->renderid = PROJTEXMAP;
	/* set triangle mesh */
	this->num_tm = 3;
	this->tm = new TriangleMesh[this->num_tm];

	/* set objects */
	char buf[256];
	getcwd(buf,256);
	strcat(buf,"/geometry/teapot1K.bin");
	this->tm[0].SetRectangleQuad(Vector3D(200.0f, 0.0f, -500.0f), Vector3D(1000.0f, 1000.0f, 1000.0f),
		Vector3D(0.7f, 0.7f, 0.7f), Vector3D(0.0f, 1.0f, 0.0f),90.0f);
	this->tm[1].SetBoundingBox(Vector3D(-200.0f, 50.0f, -300.0f),Vector3D(100.0f, 100.0f, 100.0f),
		Vector3D(1.0f, 0.5f, 0.0f));
	this->tm[1].rotationAboutAxis(this->tm[1].getCenter(),Vector3D(0.0f,1.0f,0.0f),-20.0f);
	this->tm[2].LoadBin(buf);
	this->tm[2].translate(Vector3D(0.0f,50.0f,-300.0f));

	this->tm[2].rotationAboutAxis(this->tm[2].getCenter(),Vector3D(0.0f,1.0f,0.0f),-90.0f);
	this->tm[2].reScale(Vector3D(2.0f,2.0f,2.0f));

	/* Projective texture map */
	getcwd(buf,256);
	strcat(buf,"/image/chessboard.tiff");
	this->tp = new TP(20.0f,Vector3D(-600.0f, 50.0f, -500.0f) \
		, Vector3D(1.0f,0.0f,0.0f), Vector3D(0.0f,1.0f,0.0f),buf);
	this->ppc = new PPC(this->getwidth(), this->getheight(), 55.0f);
	this->ppc->setC(this->tm[1].getCenterOfMass() + Vector3D(-400.0f, 100.0f, 500.0f));
	this->ppc->pan(-50.0f);
	this->ppc->updownTranslate(-100.0f);
	this->ppc->tilt(-10.0f);

	int framesN = 300;
	PPC *ppc = this->tp->ppc;
	//char buf[256];
	for (int fi = 0; fi < framesN; fi++) {
		float fracf = (float)fi / (float)(framesN - 1);
		ppc->translate(Vector3D(0.0f,0.0f,2.0f));
		this->tp->clearzBuffer();
		for (int i=1;i<this->num_tm;i++){
			this->tm[i].buildProjTexMap(this->tp);
		}
		renderAll();
		Fl::check();

		/* for video */
		/*sprintf(buf, "image/proj%03d.tiff",fi);
		printf("%s\n",buf);
		saveImage((const char*) buf);*/
	}
	renderAll();
}

void FrameBufferWrapper::showReflection()
{
	clearObjects();
	cout << "aa" << endl;
	this->renderid = REFLECTIONMAP;

	this->num_tm = 1;
	this->tm = new TriangleMesh[1];
	this->tm[0].LoadBin("./geometry/teapot57K.bin");
	Vector3D center = this->tm[0].getCenterOfMass();
	this->tm[0].translate(center*(-1.0f));
	Vector3D size = this->tm[0].getBoundingBox().getSize();
	float d = (size[0]>size[1])?((size[0]>size[2])?size[0]:size[2]):(size[1]>size[2]?size[1]:size[2]);

	/* CubeMap */
	this->cubemap = new CubeMap("./image/uffizi_cross.tiff");

	if (this->ppc)
		delete this->ppc;
	this->ppc = new PPC(this->fb->getwidth(), this->fb->getheight(), 90.0f);
	this->ppc->setC(Vector3D(0.0f, 0.0f, d));

	renderAll();
}

void FrameBufferWrapper::showRefraction()
{
	clearObjects();
	this->renderid = REFRACTIONMAP;

	this->num_tm = 1;
	this->tm = new TriangleMesh[1];
	this->tm[0].SetRectangleQuad(Vector3D(0.0f,0.0f,0.0f), Vector3D(100.0f,100.0f,100.0f), \
		Vector3D(1.0f,1.0f,1.0f), Vector3D(0.0f,0.0f,1.0f), 0.0f);
	this->tm[0].LoadBin("./geometry/teapot57K.bin");
	Vector3D center = this->tm[0].getCenterOfMass();
	this->tm[0].translate(center*(-1.0f));
	Vector3D size = this->tm[0].getBoundingBox().getSize();
	float d = (size[0]>size[1])?((size[0]>size[2])?size[0]:size[2]):(size[1]>size[2]?size[1]:size[2]);

	/* CubeMap */
	this->cubemap = new CubeMap("./image/uffizi_cross.tiff");

	if (this->ppc)
		delete this->ppc;
	this->ppc = new PPC(this->fb->getwidth(), this->fb->getheight(), 90.0f);
	this->ppc->setC(Vector3D(0.0f, 0.0f, d));

	renderAll();
}

void FrameBufferWrapper::renderCubeMap(PPC *currppc, FrameBuffer *currfb)
{
	int image_idx = 0;
	float u, v;
	for (int u_idx = 0; u_idx < currppc->getWidth(); u_idx++){
		for (int v_idx = 0; v_idx < currppc->getHeight(); v_idx++){
			Vector3D pt = currppc->getc() + currppc->geta() * (u_idx+0.5f) + currppc->getb() * (v_idx+0.5f);
			this->cubemap->xyz2uv(pt,&image_idx,&u,&v);
			unsigned int pixel = this->cubemap->getPixel(image_idx,u,v);
			currfb->setzBuffer(Vector3D(u_idx+0.5f,v_idx+0.5f,0.0f), pixel);
		}
	}
}

void FrameBufferWrapper::showBillBoard()
{
	clearObjects();
	this->renderid = BILLBOARD;
	this->num_tm = 2;
	this->tm = new TriangleMesh[2];
	this->tm[0].LoadBin("./geometry/teapot57K.bin");
	Vector3D center = this->tm[0].getCenterOfMass();
	this->tm[0].translate(center*(-1.0f));
	Vector3D size = this->tm[0].getBoundingBox().getSize();
	float max = (size[0]>size[1])?((size[0]>size[2])?size[0]:size[2]):(size[1]>size[2]?size[1]:size[2]);

	this->tm[1].LoadBin("./geometry/teapot1K.bin");
	this->tm[1].translate(center*(-1.0f)+Vector3D(0.0f,0.0f,-100.0f));

	/* CubeMap */
	this->cubemap = new CubeMap("./image/uffizi_cross.tiff");

	if (this->ppc)
		delete this->ppc;
	this->ppc = new PPC(this->fb->getwidth(), this->fb->getheight(), 90.0f);
	this->ppc->setC(Vector3D(0.0f, 0.0f, 100.0f));

	/* for billboard */
	this->bbfb = new FrameBuffer(u0, v0, this->fb->getwidth(), this->fb->getheight());
	this->bbppc = new PPC(this->fb->getwidth(), this->fb->getheight(), 90.f);
	this->bbppc->setC(Vector3D(0.0f, 0.0f, 0.0f));
	this->tm[1].RenderReflectionMap(this->bbppc, this->bbfb, this->cubemap);


	Vector3D newC = this->ppc->getC();
	newC.RotateArbitraryDirection(this->ppc->getb(),-135.0f);
	// commit the new values
	this->ppc->setC(newC);
	this->ppc->pan(135.0f);

	renderAll();
}

void FrameBufferWrapper::renderHW(unsigned int mode)
{

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glDisable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	// set view
	// set intrinsics
	this->ppc->setIntrinsicsHW(1.0f, 1000.0f);
	// set extrinsics
	this->ppc->setExtrinsicsHW();

	// render geometry
	for (int tmi = 0; tmi < this->num_tm; tmi++) {
		this->tm[tmi].RenderHW(mode);
	}
}

void FrameBufferWrapper::renderTexHW()
{
	Texture *tex = this->texture;
	HWTexMapInit(tex->getWidth(),tex->getHeight(),tex->getPixel(),&this->tid);
	ReshapeViewport();


	glClearColor(1.0f,1.0f,1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Setup model matrix
	glLoadIdentity();
	glRotatef(0.0f, 0.5, 1.0, 0.0); // show all sides of cube
	// Draw cube with texture assigned to each face

	glBegin(GL_QUADS);

	// Front Face
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0,  1.0,  1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,  1.0,  1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f( 1.0, -1.0,  1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0,  1.0);
	// Back Face
	/*glTexCoord2f(0.0, 1.0); glVertex3f( 1.0,  1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0,  1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f( 1.0, -1.0, -1.0);
  // Top Face
  glTexCoord2f(0.0, 1.0); glVertex3f(-1.0,  1.0, -1.0);
  glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,  1.0, -1.0);
  glTexCoord2f(1.0, 0.0); glVertex3f( 1.0,  1.0,  1.0);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1.0,  1.0,  1.0);
  // Bottom Face
  glTexCoord2f(0.0, 1.0); glVertex3f( 1.0, -1.0, -1.0);
  glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -1.0, -1.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0,  1.0);
  glTexCoord2f(0.0, 0.0); glVertex3f( 1.0, -1.0,  1.0);
  // Right face
  glTexCoord2f(0.0, 1.0); glVertex3f( 1.0,  1.0,  1.0);
  glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,  1.0, -1.0);
  glTexCoord2f(1.0, 0.0); glVertex3f( 1.0, -1.0, -1.0);
  glTexCoord2f(0.0, 0.0); glVertex3f( 1.0, -1.0,  1.0);
	// Left Face
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0,  1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0,  1.0,  1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0,  1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
	*/
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
}

void FrameBufferWrapper::Perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble xmin, xmax, ymin, ymax;
	ymax = zNear * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

void FrameBufferWrapper::ReshapeViewport()
{
	// Viewport
	glViewport(0, 0, 640, 480);
	// Projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat ratio = 640 / 480;
	Perspective(30.0, 1.0*ratio, 1.0, 30.0);
	glTranslatef(0.0, 0.0, -8.0);
	// Model view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void FrameBufferWrapper::HWTexMapInit(unsigned int width, unsigned int height, unsigned int *pixel, GLuint *tid)
{
	unsigned int w = width;
	unsigned int h = height;

	GLubyte glimg[w][h][3];
	for (int x=0; x<w; x++) {
		for (int y=0; y<h; y++) {
			//unsigned int p = pixel[y*w+x];
			unsigned int p = pixel[x*h+y];
			glimg[x][y][0] = (p & 0x000000FF);
			glimg[x][y][1] = (p & 0x0000FF00) >> 8;
			glimg[x][y][2] = (p & 0x00FF0000) >> 16;
		}
	}

	glGenTextures(1, tid);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, *tid);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, \
				0,GL_RGB, GL_UNSIGNED_BYTE, &glimg[0][0][0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glEnable(GL_TEXTURE_2D);
	// Misc OpenGL settings
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void FrameBufferWrapper::renderGPU() {

  // if the first time, call per session initialization
  if (cgi == NULL) {
    cgi = new CGInterface();
    cgi->PerSessionInit();
    soi = new ShaderOneInterface();
    soi->PerSessionInit(cgi);
		HWTexMapInit(this->bbfb->getwidth(),this->bbfb->getheight(),this->bbfb->pixel,&this->tid);
		HWCubeMapInit();
  }


  // clear the framebuffer
  glClearColor(0.0, 0.0f, 0.5f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT |
    GL_DEPTH_BUFFER_BIT);

  // set view
  // set intrinsics
  this->ppc->setIntrinsicsHW(1.0f, 1000.0f);
  // set extrinsics
  this->ppc->setExtrinsicsHW();

  // per frame initialization
  cgi->EnableProfiles();
  soi->PerFrameInit();
  soi->BindPrograms();

  // render geometry
  for (int tmi = 0; tmi < this->num_tm; tmi++) {
    this->tm[tmi].RenderHW(3);
  }

  cgi->DisableProfiles();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glShadeModel(GL_SMOOTH);
}

void FrameBufferWrapper::HWCubeMapInit()
{
	/* initialize cubemap in opengl*/
	glGenTextures(1,&this->cmid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cmid);

	unsigned int w = this->cubemap->texture[0]->getWidth();
	unsigned int h = this->cubemap->texture[0]->getHeight();

	GLubyte glimg[w][h][3];
	unsigned int *pixel;
	unsigned int p;
	for (int i=0;i<6;i++){
		/* load buffer */
		pixel = this->cubemap->texture[i]->getPixel();
		for (int x=0; x<w; x++) {
			for (int y=0; y<h; y++) {
				//unsigned int p = pixel[y*w+x];
				p = pixel[x*h+y];
				glimg[x][y][0] = (p & 0x000000FF);
				glimg[x][y][1] = (p & 0x0000FF00) >> 8;
				glimg[x][y][2] = (p & 0x00FF0000) >> 16;
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0,GL_RGB, w, h, \
				0,GL_RGB, GL_UNSIGNED_BYTE, &glimg[0][0][0]);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_CUBE_MAP);
	// Misc OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

// HW6 demo
void FrameBufferWrapper::demoHWFilledMode()
{
	clearObjects();
	this->num_tm = 1;
	this->tm = new TriangleMesh[1];
	this->tm[0].LoadBin("./geometry/teapot1K.bin");
	Vector3D center = this->tm[0].getCenterOfMass();
	this->tm[0].translate(center*(-1.0f));
	this->ppc = new PPC(this->fb->getwidth(), this->fb->getheight(), 55.0f);
	this->ppc->setC(Vector3D(0.0f,0.0f,200.0f));
	this->renderid = FILLED;
	renderAll();
}
void FrameBufferWrapper::demoHWWireFrameMode()
{
	clearObjects();
	this->num_tm = 1;
	this->tm = new TriangleMesh[1];
	this->tm[0].LoadBin("./geometry/teapot1K.bin");
	Vector3D center = this->tm[0].getCenterOfMass();
	this->tm[0].translate(center*(-1.0f));
	this->ppc = new PPC(this->fb->getwidth(), this->fb->getheight(), 55.0f);
	this->ppc->setC(Vector3D(0.0f,0.0f,200.0f));
	this->renderid = WIREFRAME;
	renderAll();
}

void FrameBufferWrapper::demoHWTexMap()
{
	clearObjects();
	this->texture = new Texture[1];
	this->readImage2Texture("./image/palace256.tiff",this->texture);

	this->renderid = TEXMAP;
	renderAll();
}

void FrameBufferWrapper::demoGPUShader()
{
	clearObjects();
	this->renderid = GPUSHADER;
	this->num_tm = 2;
	this->tm = new TriangleMesh[2];
	this->tm[0].LoadBin("./geometry/teapot57K.bin");
	Vector3D center = this->tm[0].getCenterOfMass();
	this->tm[0].translate(center*(-1.0f));
	this->tm[1].LoadBin("./geometry/teapot57K.bin");
	center = this->tm[1].getCenterOfMass();
	this->tm[1].translate(center*(-1.0f)+Vector3D(0.0f,0.0f,-100.0f));
	this->ppc = new PPC(this->fb->getwidth(), this->fb->getheight(), 90.0f);
	this->ppc->setC(Vector3D(0.0f, 0.0f, 100.0f));
	this->bbfb = new FrameBuffer(u0, v0, this->fb->getwidth(), this->fb->getheight());
	this->bbppc = new PPC(this->fb->getwidth(), this->fb->getheight(), 90.0f);
	this->bbppc->setC(Vector3D(0.0f, 0.0f, 0.0f));
	this->bbfb->clear(0xFFFFFFFF,0.0f);

	this->cubemap = new CubeMap("./image/uffizi_cross.tiff");
	renderCubeMap(this->bbppc, this->bbfb);
	this->tm[1].RenderReflectionMap(this->bbppc, this->bbfb,this->cubemap);

	Vector3D newC = this->ppc->getC();
	newC.RotateArbitraryDirection(ppc->getb(),-135.0f);
	// commit the new values
	ppc->setC(newC);
	ppc->pan(135.0f);

	renderAll();
}
