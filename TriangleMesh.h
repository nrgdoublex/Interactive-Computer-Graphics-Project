#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "Vector3D.h"
#include "Matrix3D.h"
#include "FrameBuffer.h"
#include "PPC.h"
#include "AABB.h"
#include "Texture.h"
#include "Mipmap.h"
#include "PSL.h"
#include "TP.h"
#include "CubeMap.h"
#include <limits>
#include <cfloat>
#include <cmath>

using namespace std;

#define QUAD (1)

// for HW rendering mode
#define FILLED 		(0)
#define WIREFRAME (1)
#define TEXMAP    (2)
#define GPUSHADER (3)

class TriangleMesh {

public:
	Vector3D *vertices, *cols, *normals;		// cols for color
	int num_vert;
	unsigned int *triangles;
	int num_tri;

	TriangleMesh() : vertices(0), cols(0), num_vert(0), triangles(0), num_tri(0), normals(0), bBox(){};
  ~TriangleMesh();
	void SetBoundingBox(Vector3D O, Vector3D dims, Vector3D color);
	void Allocate(int vsN, int tsN);
	void RenderPoints(PPC *ppc, FrameBuffer *fb);
	void RenderWireframe(PPC *ppc, FrameBuffer *fb);							// part f.
	void RenderFilledIS(PPC *ppc, FrameBuffer *fb, PSL *lighting);									// part g.
	void RenderFilledIS(PPC *ppc, FrameBuffer *fb);
	Vector3D getCenterOfMass();
	void setColor(int idx, Vector3D rgb);
	unsigned int getColor(int idx);
	void LoadBin(const char *fname);															// part a.
	AABB getBoundingBox();																				// part b.
	Vector3D getCenter();
	void translate(Vector3D shift);																// part c.
	void reScaleAndPosition(Vector3D newcom, AABB newbbox);				// part e.
	void reScaleAndPosition(Vector3D newcom, Vector3D newsize);		// part e.
	void reScale(Vector3D scale_ratio);														// part d.
	void rotationAboutAxis(Vector3D O, Vector3D axis, float degree);
	Vector3D setSidednessEQ(Vector3D v0, Vector3D v1, Vector3D v2);
	void RenderFilledMS(PPC *ppc, FrameBuffer *fb, PSL *lighting);
	void RenderTexture(PPC *ppc, FrameBuffer *fb, Texture *texture, float scale);
	void SetRectangleQuad(Vector3D O, Vector3D dims, Vector3D color, Vector3D axis, float degree);
	void RenderTextureQuad(PPC *ppc, FrameBuffer *fb, Texture *texture, float scale);
	void RenderTextureQuad(PPC *ppc, FrameBuffer *fb, Mipmap *mipmap, float scale);
	void buildShadowMap(PSL *psl);
	void buildProjTexMap(TP *tp);
	void RenderProjMap(PPC *ppc, FrameBuffer *fb, TP *tp);
	void RenderReflectionMap(PPC *ppc, FrameBuffer *fb, CubeMap *cubemap);
	Matrix3D computeMSIM(PPC *ppc, Matrix3D vs);
	Matrix3D computeSSIM(Vector3D *pvs);
	void RenderRefractionMap(PPC *ppc, FrameBuffer *fb, CubeMap *cubemap);
	void RenderBillBoard(PPC *ppc, FrameBuffer *fb, PPC *bbppc, FrameBuffer *bb, CubeMap *cubemap);
	void RenderHW(unsigned int mode);



private:
	AABB bBox;
	Vector3D com;
	unsigned int quad;
};

#endif
