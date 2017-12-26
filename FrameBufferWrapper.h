#ifndef FrameBufferWrapper_H
#define FrameBufferWrapper_H

#include <sstream>
#include <iostream>
#include <tiffio.h>
#include <tiffio.hxx>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include "gui.h"
#include "FrameBuffer.h"
#include "TriangleMesh.h"
#include "Texture.h"
#include "PSL.h"
#include "CubeMap.h"
#include "CGInterface.h"

#define SHADOWMAP 				(1)
#define PROJTEXMAP 				(2)
#define REFLECTIONMAP 		(3)
#define REFRACTIONMAP 		(4)
#define BILLBOARD 				(5)

class FrameBufferWrapper {
public:
	int renderid;
	GLuint tid, cmid;
	unsigned int num_tm;
	TriangleMesh *tm;
	PPC *ppc, *ppc3;
	PPC *bbppc;

  /* CGI interface */
  CGInterface *cgi;
  ShaderOneInterface *soi;

	MainUI *gui;
	FrameBuffer *fb, *fb3;
	FrameBuffer *bbfb;
	Vector3D *billboard;
	FrameBufferWrapper(unsigned int width, unsigned int height);
	~FrameBufferWrapper();
	void debug();
	void render(FrameBuffer *currfb, PPC *currppc);
	void render(FrameBuffer *currfb, PPC *currppc, PPC *targetPPC, int trdperson);
	void renderAll();		//wrapper function
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
	int readImage2Texture(const char *filepath, Texture *texture);
	void animation();
	void setFocal(float flen);
	void interpolation();
	void translateTM(float x, float y, float z);
	void scaleTM(float sfx, float sfy, float sfz);
	void rePositionScaleTM(float cx, float cy, float cz, float sfx, float sfy, float sfz);
	void switchFilledWireframe();
	void loadBin(const char *filepath);
	void showTexture(const char *filepath, float scale, unsigned int width, unsigned int height);
	void showMipmap(const char *filepaths[], float scale, unsigned int width, unsigned int height, unsigned int num_seqs);
	void renderTexture(FrameBuffer *currfb, PPC *currppc, float *scale);
	void renderMipmap(FrameBuffer *currfb, PPC *currppc, float scale);
	void renderModelInterpolation(FrameBuffer *currfb, PPC *currppc);
	void modelInterpolation();
	void clearObjects();
	void showShadowMap();
	void showProjTexMap();
	void showReflection();
	void showRefraction();
	void renderCubeMap(PPC *currppc, FrameBuffer *currfb);
	void showBillBoard();
	void renderHW(unsigned int mode);
	void renderTexHW();
	void ReshapeViewport();
	void Perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void HWTexMapInit(unsigned int width, unsigned int height, unsigned int *pixel, GLuint *tid);
  void renderGPU();
	void HWCubeMapInit();

	// HW6 demo
	void demoHWFilledMode();
	void demoHWWireFrameMode();
	void demoHWTexMap();
	void demoGPUShader();

private:
	unsigned int u0;
	unsigned int v0;
	unsigned int switch_flag;
	Texture *texture;
	float *t_scale;
	Mipmap *mipmap;
	PSL *psl;
	TP *tp;
	CubeMap *cubemap;

};

extern FrameBufferWrapper *wrapper;

#endif
