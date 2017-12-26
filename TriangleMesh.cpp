#include "TriangleMesh.h"

/*-----------------------------------------------------
  Destructor
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:

-----------------------------------------------------*/
TriangleMesh::~TriangleMesh()
{
    delete [] this->vertices;
    delete [] this->cols;
    delete [] this->triangles;
    delete [] this->normals;
}

/*-----------------------------------------------------
  Allocate vertices and triangles
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  int vsN: number of vertices
  int tsN: number of triangles
Return:

-----------------------------------------------------*/
void TriangleMesh::Allocate(int vsN, int tsN)
{

	this->num_vert = vsN;
	this->vertices = new Vector3D[this->num_vert];
	this->cols = new Vector3D[this->num_vert];
	this->num_tri = tsN;
	this->triangles = new unsigned[3*this->num_tri];

}

/*-----------------------------------------------------
  Setting cube mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D O:     center of mass
  Vector3D dims:  size of cube
  Vector3D color: reference for assigning color to each vertex
Return:

-----------------------------------------------------*/
void TriangleMesh::SetBoundingBox(Vector3D O, Vector3D dims, Vector3D color)
{
	int vertsN = 8;
	int trisN = 6 * 2;
	this->Allocate(vertsN, trisN);

	for (int vi = 0; vi < vertsN/2; vi++) {
		cols[vi] = color;
		cols[vi + vertsN / 2] = color*0.0f;
	}

  int vi = 0;
  this->vertices[vi++] = O + Vector3D(-dims[0] / 2.0f, +dims[1] / 2.0f, +dims[2] / 2.0f);
  this->vertices[vi++] = O + Vector3D(-dims[0] / 2.0f, -dims[1] / 2.0f, +dims[2] / 2.0f);
  this->vertices[vi++] = O + Vector3D(+dims[0] / 2.0f, -dims[1] / 2.0f, +dims[2] / 2.0f);
  this->vertices[vi++] = O + Vector3D(+dims[0] / 2.0f, +dims[1] / 2.0f, +dims[2] / 2.0f);
  this->vertices[vi++] = O + Vector3D(-dims[0] / 2.0f, +dims[1] / 2.0f, -dims[2] / 2.0f);
  this->vertices[vi++] = O + Vector3D(-dims[0] / 2.0f, -dims[1] / 2.0f, -dims[2] / 2.0f);
  this->vertices[vi++] = O + Vector3D(+dims[0] / 2.0f, -dims[1] / 2.0f, -dims[2] / 2.0f);
  this->vertices[vi++] = O + Vector3D(+dims[0] / 2.0f, +dims[1] / 2.0f, -dims[2] / 2.0f);

  for (int i=0;i<this->num_vert;i++)
    this->bBox.addPoint(this->vertices[i]);

	trisN = 0;
	int tri = 0;

	this->triangles[tri * 3 + 0] = 0;
	this->triangles[tri * 3 + 1] = 1;
	this->triangles[tri * 3 + 2] = 2;
	tri++;
	this->triangles[tri * 3 + 0] = 2;
	this->triangles[tri * 3 + 1] = 3;
	this->triangles[tri * 3 + 2] = 0;
	tri++;
	trisN += 2;

	this->triangles[tri * 3 + 0] = 3;
	this->triangles[tri * 3 + 1] = 2;
	this->triangles[tri * 3 + 2] = 6;
	tri++;
	this->triangles[tri * 3 + 0] = 6;
	this->triangles[tri * 3 + 1] = 7;
	this->triangles[tri * 3 + 2] = 3;
	tri++;
	trisN += 2;

	this->triangles[tri * 3 + 0] = 7;
	this->triangles[tri * 3 + 1] = 6;
	this->triangles[tri * 3 + 2] = 5;
	tri++;
	this->triangles[tri * 3 + 0] = 5;
	this->triangles[tri * 3 + 1] = 4;
	this->triangles[tri * 3 + 2] = 7;
	tri++;
	trisN += 2;

	this->triangles[tri * 3 + 0] = 4;
	this->triangles[tri * 3 + 1] = 5;
	this->triangles[tri * 3 + 2] = 1;
	tri++;
	this->triangles[tri * 3 + 0] = 1;
	this->triangles[tri * 3 + 1] = 0;
	this->triangles[tri * 3 + 2] = 4;
	tri++;
	trisN += 2;


	this->triangles[tri * 3 + 0] = 5;
	this->triangles[tri * 3 + 1] = 6;
	this->triangles[tri * 3 + 2] = 2;
	tri++;
	this->triangles[tri * 3 + 0] = 2;
	this->triangles[tri * 3 + 1] = 1;
	this->triangles[tri * 3 + 2] = 5;
	tri++;
	trisN += 2;

	this->triangles[tri * 3 + 0] = 0;
	this->triangles[tri * 3 + 1] = 3;
	this->triangles[tri * 3 + 2] = 7;
	tri++;
	this->triangles[tri * 3 + 0] = 7;
	this->triangles[tri * 3 + 1] = 4;
	this->triangles[tri * 3 + 2] = 0;
	tri++;
	trisN += 2;

  this->com = O;
}

/*-----------------------------------------------------
  Mark vertices of triangle mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  PPC *ppc:         camera with respect to
  FrameBuffer *fb:  framebuffer to render
Return:

-----------------------------------------------------*/
void TriangleMesh::RenderPoints(PPC *ppc, FrameBuffer *fb)
{
	for (int vi = 0; vi < this->num_vert; vi++) {
		Vector3D projV;
		if (!ppc->project(this->vertices[vi], projV))
			continue;
		unsigned int vcolor = 0xFF0000FF;
		fb->drawCircle((int) projV[0], (int) projV[1], 2, getColor(vi));
	}
}

/*-----------------------------------------------------
  Rendering wireframes of triangle mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  PPC *ppc:         camera with respect to
  FrameBuffer *fb:  framebuffer to render
Return:

-----------------------------------------------------*/
void TriangleMesh::RenderWireframe(PPC *ppc, FrameBuffer *fb)
{
	for (int tri = 0; tri < this->num_tri; tri++) {
		for (int ei = 0; ei < 3; ei++) {
			int ei1 = (ei + 1) % 3;
			Vector3D V0 = this->vertices[this->triangles[3 * tri + ei]];
			Vector3D V1 = this->vertices[this->triangles[3 * tri + ei1]];
			Vector3D c0 = this->cols[this->triangles[3 * tri + ei]];
			Vector3D c1 = this->cols[this->triangles[3 * tri + ei1]];
			fb->draw3DSegment(V0, V1, c0, c1, ppc);
		}
	}
}

Vector3D TriangleMesh::setSidednessEQ(Vector3D v0, Vector3D v1, Vector3D v2)
{

	Vector3D ret;
	ret[0] = v1[1] - v0[1];
	ret[1] = v0[0] - v1[0];
	ret[2] = -v0[0] * ret[0] + v0[1] * (-ret[1]);

	Vector3D v2p(v2[0], v2[1], 1.0f);
	if (v2p*ret < 0.0f)
		ret = ret * (-1.0f);

	return ret;
}


/*-----------------------------------------------------
  Rendering filled triangle mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  PPC *ppc:         camera with respect to
  FrameBuffer *fb:  framebuffer to render
Return:

-----------------------------------------------------*/
void TriangleMesh::RenderFilledIS(PPC *ppc, FrameBuffer *fb, PSL *lighting)
{
  float x0, y0, x1, y1, x2, y2;
  float width = fb->getwidth();
  float height = fb->getheight();

  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
	for (int vi = 0; vi < this->num_vert; vi++) {
		if (!ppc->project(this->vertices[vi], projV[vi]))
			projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	}

  for (int tri=0;tri<this->num_tri;tri++){
    /* get projected triangles */
    Vector3D pvs[3];
    Vector3D col[3];
		pvs[0] = projV[this->triangles[3 * tri + 0]];
		pvs[1] = projV[this->triangles[3 * tri + 1]];
		pvs[2] = projV[this->triangles[3 * tri + 2]];
    col[0] = this->cols[this->triangles[3 * tri + 0]];
    col[1] = this->cols[this->triangles[3 * tri + 1]];
    col[2] = this->cols[this->triangles[3 * tri + 2]];
		if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX)
			continue;
    Vector3D eqs[3];
    for (int i = 0; i < 3; i++) {
			eqs[i] = this->setSidednessEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
		}

    x0 = pvs[0][0]; y0 = pvs[0][1];
    x1 = pvs[1][0]; y1 = pvs[1][1];
    x2 = pvs[2][0]; y2 = pvs[2][1];
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
    Matrix3D ISI;
    ISI.SetColumn(Vector3D(pvs[0][0], pvs[1][0], pvs[2][0]),0);
		ISI.SetColumn(Vector3D(pvs[0][1], pvs[1][1], pvs[2][1]),1);
		ISI.SetColumn(Vector3D(1.0f, 1.0f, 1.0f),2);
    try{
		  ISI = ISI.Inverse();
    }
    catch (const char *msg){
      printf("%s\n",msg);
    }
    Vector3D redABC = ISI*Vector3D(col[0][0], col[1][0], col[2][0]);
		Vector3D greenABC = ISI*Vector3D(col[0][1], col[1][1], col[2][1]);
		Vector3D blueABC = ISI*Vector3D(col[0][2], col[1][2], col[2][2]);
		Vector3D zABC = ISI*Vector3D(pvs[0][2], pvs[1][2], pvs[2][2]);

    for (curr_y = top; curr_y <= bottom; curr_y++){
      for (curr_x = left; curr_x <= right; curr_x++){
        Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
				if (currPix * eqs[0] < 0.0f || currPix * eqs[1] < 0.0f
					|| currPix * eqs[2] < 0.0f)
					continue;
				Vector3D vc(currPix*redABC, currPix*greenABC, currPix*blueABC);
				currPix[2] = currPix*zABC;

        Vector3D proj = ppc->c + ppc->a * currPix[0] + ppc->b * currPix[1];
        fb->setzBuffer(currPix, fb->getColorFromVector(vc));
      }
    }
  }

  delete [] projV;
}

void TriangleMesh::RenderFilledIS(PPC *ppc, FrameBuffer *fb)
{
  float x0, y0, x1, y1, x2, y2;
  float width = fb->getwidth();
  float height = fb->getheight();

  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
	for (int vi = 0; vi < this->num_vert; vi++) {
		if (!ppc->project(this->vertices[vi], projV[vi]))
			projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	}

  for (int tri=0;tri<this->num_tri;tri++){
    /* get projected triangles */
    Vector3D pvs[3];
    Vector3D col[3];
		pvs[0] = projV[this->triangles[3 * tri + 0]];
		pvs[1] = projV[this->triangles[3 * tri + 1]];
		pvs[2] = projV[this->triangles[3 * tri + 2]];
    col[0] = this->cols[this->triangles[3 * tri + 0]];
    col[1] = this->cols[this->triangles[3 * tri + 1]];
    col[2] = this->cols[this->triangles[3 * tri + 2]];
		if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX)
			continue;
    Vector3D eqs[3];
    for (int i = 0; i < 3; i++) {
			eqs[i] = this->setSidednessEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
		}

    x0 = pvs[0][0]; y0 = pvs[0][1];
    x1 = pvs[1][0]; y1 = pvs[1][1];
    x2 = pvs[2][0]; y2 = pvs[2][1];
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
    Matrix3D ISI;
    ISI.SetColumn(Vector3D(pvs[0][0], pvs[1][0], pvs[2][0]),0);
		ISI.SetColumn(Vector3D(pvs[0][1], pvs[1][1], pvs[2][1]),1);
		ISI.SetColumn(Vector3D(1.0f, 1.0f, 1.0f),2);
    try{
		  ISI = ISI.Inverse();
    }
    catch (const char *msg){
      printf("%s\n",msg);
    }
    Vector3D redABC = ISI*Vector3D(col[0][0], col[1][0], col[2][0]);
		Vector3D greenABC = ISI*Vector3D(col[0][1], col[1][1], col[2][1]);
		Vector3D blueABC = ISI*Vector3D(col[0][2], col[1][2], col[2][2]);
		Vector3D zABC = ISI*Vector3D(pvs[0][2], pvs[1][2], pvs[2][2]);

    for (curr_y = top; curr_y <= bottom; curr_y++){
      for (curr_x = left; curr_x <= right; curr_x++){
        Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
				if (currPix * eqs[0] < 0.0f || currPix * eqs[1] < 0.0f
					|| currPix * eqs[2] < 0.0f)
					continue;
				Vector3D vc(currPix*redABC, currPix*greenABC, currPix*blueABC);
				currPix[2] = currPix*zABC;

        Vector3D proj = ppc->c + ppc->a * currPix[0] + ppc->b * currPix[1];
        fb->setzBuffer(currPix, fb->getColorFromVector(vc));
      }
    }
  }

  delete [] projV;
}

/*-----------------------------------------------------
  Getting center of mass of triangle mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
  Vector3D: center of mass
-----------------------------------------------------*/
Vector3D TriangleMesh::getCenterOfMass() {

	Vector3D ret(0.0f, 0.0f, 0.0f);
	for (int vi = 0; vi < this->num_vert; vi++) {
		ret = ret + this->vertices[vi];
	}
	ret = ret / (float)this->num_vert;
  this->com = ret;
	return ret;

}

/*-----------------------------------------------------
  Setting color to a specific vertex in triangle mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  int idx:        index of vertex
  Vector3D rgb:   color vector(blue,green,red)
Return:

-----------------------------------------------------*/
void TriangleMesh::setColor(int idx, Vector3D rgb)
{
  for (int i=0;i<3;i++){
    int component = rgb[i];
    component = (component < 0) ? 0 : component;
		component = (component > 1) ? 1 : component;
    this->cols[idx][i] = component;
  }
}

/*-----------------------------------------------------
  Getting color of a specific vertex in triangle mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  int idx:        index of vertex
Return:
  Vector3D:   color vector(blue,green,red)
-----------------------------------------------------*/
unsigned int TriangleMesh::getColor(int idx)
{
	unsigned int ret = 0xFF000000;
	unsigned char *rgba = (unsigned char*)&ret;
	for (int i = 0; i < 3; i++) {
		int ichan = (int)(255.0f*this->cols[idx][i]);
    ichan = (ichan < 0) ? 0 : ichan;
		ichan = (ichan > 255) ? 255 : ichan;
		rgba[i] = ichan;
	}
	return ret;
}

/*-----------------------------------------------------
  Loading mesh from file
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  const char *fname: file name
Return:

-----------------------------------------------------*/
void TriangleMesh::LoadBin(const char *fname) {

	ifstream ifs(fname, ios::binary);
	if (ifs.fail()) {
		cerr << "INFO: cannot open file: " << fname << endl;
		return;
	}

	ifs.read((char*)&this->num_vert, sizeof(int));
	char yn;
	ifs.read(&yn, 1); // always xyz
	if (yn != 'y') {
		cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
		return;
	}
	if (this->vertices)
		delete this->vertices;
	this->vertices = new Vector3D[this->num_vert];

	ifs.read(&yn, 1); // cols 3 floats
	if (this->cols)
		delete this->cols;
	this->cols = 0;
	if (yn == 'y') {
		this->cols = new Vector3D[this->num_vert];
	}

	ifs.read(&yn, 1); // normals 3 floats
	//Vector3D *normals = 0; // don't have normals for now
	if (this->normals)
		delete this->normals;
	this->normals = NULL;
	if (yn == 'y') {
		this->normals = new Vector3D[this->num_vert];
	}

	ifs.read(&yn, 1); // texture coordinates 2 floats
	float *tcs = 0; // don't have texture coordinates for now
	if (tcs)
		delete tcs;
	tcs = 0;
	if (yn == 'y') {
		tcs = new float[this->num_vert * 2];
	}

	ifs.read((char*)this->vertices, this->num_vert * 3 * sizeof(float)); // load verts

	if (this->cols) {
		ifs.read((char*)this->cols, this->num_vert * 3 * sizeof(float)); // load cols
	}

	if (this->normals)
		ifs.read((char*)normals, this->num_vert * 3 * sizeof(float)); // load normals

	if (tcs)
		ifs.read((char*)tcs, this->num_vert * 2 * sizeof(float)); // load texture coordinates

	ifs.read((char*)&this->num_tri, sizeof(int));
	if (this->triangles)
		delete this->triangles;
	this->triangles = new unsigned int[this->num_tri * 3];
	ifs.read((char*)this->triangles, this->num_tri * 3 * sizeof(unsigned int)); // read triangles

	ifs.close();

	cerr << "INFO: loaded " << this->num_vert << " verts, " << this->num_tri << " tris from " << endl << "      " << fname << endl;
	cerr << "      xyz " << ((cols) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

  /* get bounding box size */
  this->bBox.reset();
  for (int i=0;i<this->num_vert;i++){
    this->bBox.addPoint(this->vertices[i]);
  }
  /* get center of mass */
  this->com = this->getCenterOfMass();

	delete[] tcs;
  //delete[] normals;
}

/*-----------------------------------------------------
  Getting bounding box of triangle mesh
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
  AABB: bounding box
-----------------------------------------------------*/
AABB TriangleMesh::getBoundingBox()
{
  this->bBox.reset();
  for (int i=0;i<this->num_vert;i++)
    this->bBox.addPoint(this->vertices[i]);
  return this->bBox;
}

Vector3D TriangleMesh::getCenter()
{
  return this->com;
}

/*-----------------------------------------------------
  Moving triangle mesh by some vector
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D shift: vector by which to translate
Return:

-----------------------------------------------------*/
void TriangleMesh::translate(Vector3D shift)
{
  if (!this->vertices)
    return;
  for (int i=0;i<this->num_vert;i++)
    this->vertices[i] = this->vertices[i] + shift;
  this->com = this->com + shift;
  return;
}

/*-----------------------------------------------------
  placing centroid in some vector and rescale
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D newcom:  new centroid for triangle mesh
  AABB newbbox:     size of bounding box
Return:

-----------------------------------------------------*/
void TriangleMesh::reScaleAndPosition(Vector3D newcom, AABB newbbox)
{
  if (!this->vertices)
    return;
  /* recalculate COM and size first */
  Vector3D oldcom = this->getCenterOfMass();
  Vector3D oldsize = this->getBoundingBox().getSize();
  Vector3D newsize = newbbox.getSize();

  float ratio_x = (float) newsize[0]/oldsize[0];
  float ratio_y = (float) newsize[1]/oldsize[1];
  float ratio_z = (float) newsize[2]/oldsize[2];

  /* scale and translate */
  for (int i=0;i<this->num_vert;i++){
    Vector3D &point = this->vertices[i];
    Vector3D scaled(newcom[0]+(point[0]-oldcom[0])*ratio_x \
                  , newcom[1]+(point[1]-oldcom[1])*ratio_y \
                  , newcom[2]+(point[2]-oldcom[2])*ratio_z);
    this->vertices[i] = scaled;
  }
}

/*-----------------------------------------------------
  placing centroid in some vector and rescale
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D newcom:      new centroid for triangle mesh
  Vector3D newsize:     size of bounding box
Return:

-----------------------------------------------------*/
void TriangleMesh::reScaleAndPosition(Vector3D newcom, Vector3D newsize)
{
  if (!this->vertices)
    return;
  /* recalculate COM and size first */
  Vector3D oldcom = this->getCenterOfMass();
  Vector3D oldsize = this->getBoundingBox().getSize();

  float ratio_x = (float) newsize[0]/oldsize[0];
  float ratio_y = (float) newsize[1]/oldsize[1];
  float ratio_z = (float) newsize[2]/oldsize[2];

  /* scale and translate */
  for (int i=0;i<this->num_vert;i++){
    Vector3D &point = this->vertices[i];
    Vector3D scaled(newcom[0]+(point[0]-oldcom[0])*ratio_x \
                  , newcom[1]+(point[1]-oldcom[1])*ratio_y \
                  , newcom[2]+(point[2]-oldcom[2])*ratio_z);
    this->vertices[i] = scaled;
  }
}

/*-----------------------------------------------------
  Rescaling triangle mesh w.r.t. centroid
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D scale_ratio: scale factor
Return:

-----------------------------------------------------*/
void TriangleMesh::reScale(Vector3D scale_ratio)
{
  if (!this->vertices)
    return;
  Vector3D com = this->getCenterOfMass();
  Vector3D size = this->getBoundingBox().getSize();
  Vector3D newsize(size[0]*scale_ratio[0],size[1]*scale_ratio[1],size[2]*scale_ratio[2]);
  this->reScaleAndPosition(com,newsize);
}

/*-----------------------------------------------------
  Rotation of triangle mesh about some point along some axis
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D O:     point about which to rotate
  Vector3D axis:  axis along which to rotate
  float degree:   degree of rotation
Return:

-----------------------------------------------------*/
void TriangleMesh::rotationAboutAxis(Vector3D O, Vector3D axis, float degree)
{
  for (int vi = 0; vi < this->num_vert; vi++) {
		this->vertices[vi].RotateArbitraryAxis(O, axis, degree);
	}
}

void TriangleMesh::RenderFilledMS(PPC *ppc, FrameBuffer *fb, PSL *lighting)
{
  float x0, y0, x1, y1, x2, y2;
  float width = fb->getwidth();
  float height = fb->getheight();

  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
	for (int vi = 0; vi < this->num_vert; vi++) {
		if (!ppc->project(this->vertices[vi], projV[vi]))
			projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	}

  for (int tri=0;tri<this->num_tri;tri++){
    /* get projected triangles */
    Vector3D pvs[3];
    Vector3D col[3];
    Vector3D vs[3];
    vs[0] = this->vertices[this->triangles[3 * tri + 0]];
		vs[1] = this->vertices[this->triangles[3 * tri + 1]];
		vs[2] = this->vertices[this->triangles[3 * tri + 2]];
		pvs[0] = projV[this->triangles[3 * tri + 0]];
		pvs[1] = projV[this->triangles[3 * tri + 1]];
		pvs[2] = projV[this->triangles[3 * tri + 2]];
    col[0] = this->cols[this->triangles[3 * tri + 0]];
    col[1] = this->cols[this->triangles[3 * tri + 1]];
    col[2] = this->cols[this->triangles[3 * tri + 2]];
		if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX)
			continue;
    Vector3D eqs[3];
    for (int i = 0; i < 3; i++) {
			eqs[i] = this->setSidednessEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
		}

    x0 = pvs[0][0]; y0 = pvs[0][1];
    x1 = pvs[1][0]; y1 = pvs[1][1];
    x2 = pvs[2][0]; y2 = pvs[2][1];
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

    // normal of triangle
    Vector3D normal = ((vs[1]-vs[0]).CrossProduct(vs[2]-vs[1])).Normalize();

    // for model space color interpolation
    Matrix3D MSIM;
    MSIM.SetColumn(vs[0],0);
		MSIM.SetColumn(vs[1],1);
		MSIM.SetColumn(vs[2],2);
    try{
		  MSIM = MSIM.Inverse();
    }
    catch (const char *msg){
      continue;
    }
    Matrix3D ISIM;
    ISIM.SetColumn(Vector3D(pvs[0][0], pvs[1][0], pvs[2][0]),0);
		ISIM.SetColumn(Vector3D(pvs[0][1], pvs[1][1], pvs[2][1]),1);
		ISIM.SetColumn(Vector3D(1.0f, 1.0f, 1.0f),2);
    try{
		  ISIM = ISIM.Inverse();
    }
    catch (const char *msg){
      continue;
    }
		Vector3D zABC = ISIM*Vector3D(pvs[0][2], pvs[1][2], pvs[2][2]);

    // set color
    int curr_x, curr_y;
    for (curr_y = top; curr_y <= bottom; curr_y++){
      for (curr_x = left; curr_x <= right; curr_x++){
        Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
				if (currPix * eqs[0] < 0.0f || currPix * eqs[1] < 0.0f
					|| currPix * eqs[2] < 0.0f)
					continue;
        currPix[2] = currPix*zABC;
        Vector3D proj = ppc->c + ppc->a * currPix[0] + ppc->b * currPix[1];
        Vector3D bary_coor = (MSIM * (ppc->C + proj / currPix[2]));
        Vector3D cv = (col[0]*bary_coor[0]+col[1]*bary_coor[1]+col[2]*bary_coor[2]);

        if (!lighting->renderOrNot(ppc->C + proj / currPix[2]))
          fb->setzBuffer(currPix, 0xFF000000);
        else
				  fb->setzBuffer(currPix, fb->getColorFromVector(cv));
      }
    }
  }

  delete [] projV;
}

void TriangleMesh::RenderTexture(PPC *ppc, FrameBuffer *fb, Texture *texture, float scale)
{
  float x0, y0, x1, y1, x2, y2;
  float width = fb->getwidth();
  float height = fb->getheight();

  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
  for (int vi = 0; vi < this->num_vert; vi++) {
    if (!ppc->project(this->vertices[vi], projV[vi]))
      projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
  }

  Vector3D pvs[3];
  Vector3D vs[3];
  Vector3D st[3];
  for (int tri=0;tri<this->num_tri;tri++){
    /* get projected triangles */
    vs[0] = this->vertices[this->triangles[3 * tri + 0]];
    vs[1] = this->vertices[this->triangles[3 * tri + 1]];
    vs[2] = this->vertices[this->triangles[3 * tri + 2]];
    pvs[0] = projV[this->triangles[3 * tri + 0]];
    pvs[1] = projV[this->triangles[3 * tri + 1]];
    pvs[2] = projV[this->triangles[3 * tri + 2]];
    // texel coor
    st[0] = Vector3D(0.0f,0.0f,0.0f);
    st[1] = Vector3D(scale,0.0f,0.0f);
    st[2] = Vector3D(0.0f,scale,0.0f);
    if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX)
      continue;
    Vector3D eqs[3];
    for (int i = 0; i < 3; i++) {
      eqs[i] = this->setSidednessEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
    }

    x0 = pvs[0][0]; y0 = pvs[0][1];
    x1 = pvs[1][0]; y1 = pvs[1][1];
    x2 = pvs[2][0]; y2 = pvs[2][1];
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

    // normal of triangle
    Vector3D normal = (vs[1]-vs[0]).CrossProduct(vs[2]-vs[1]).Normalize();

    // for model space color interpolation
    Matrix3D cart2bary;
    cart2bary.SetColumn(vs[0],0);
    cart2bary.SetColumn(vs[1],1);
    cart2bary.SetColumn(vs[2],2);
    try{
      cart2bary = cart2bary.Inverse();
    }
    catch (const char *msg){
      printf("%s\n",msg);
    }
    // set color
    int curr_x, curr_y;
    for (curr_y = top; curr_y <= bottom; curr_y++){
      for (curr_x = left; curr_x <= right; curr_x++){
        Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
        if (currPix * eqs[0] < 0.0f || currPix * eqs[1] < 0.0f
          || currPix * eqs[2] < 0.0f)
          continue;
        Vector3D proj = ppc->c + ppc->a * ((float)curr_x + .5f) + ppc->b * ((float)curr_y + .5f);
        float lambda = (float) (normal * (vs[1] - ppc->C)) / (normal * proj);
        // bary coordinates
        Vector3D bary_coor = (cart2bary * (ppc->C + proj * lambda)).Normalize();
        // color
        Vector3D st_coor = st[0]*bary_coor[1] + st[1]*bary_coor[2] + st[2]*bary_coor[0];
        unsigned int color = texture->getPixel(fmod(st_coor[0],scale),fmod(st_coor[1],scale));

        //currPix[2] = pvs[0][2]*bary_coor[0] + pvs[1][2]*bary_coor[1] + pvs[2][2]*bary_coor[2];
        currPix[2] = 1.0f / ((1.0f / pvs[0][2])*bary_coor[0] + (1.0f / pvs[1][2])*bary_coor[1] \
                    + (1.0f / pvs[2][2])*bary_coor[2]);
        fb->setzBuffer(currPix, color);
      }
    }
  }

  delete [] projV;
}

void TriangleMesh::SetRectangleQuad(Vector3D O, Vector3D dims, Vector3D color, Vector3D axis, float degree)
{
  this->quad = QUAD;
  int vertsN = 4;
	int trisN = 2;
	this->Allocate(vertsN, trisN);
  this->normals = new Vector3D[this->num_vert];

	for (int vi = 0; vi < vertsN; vi++) {
		this->cols[vi] = color;
	}

  int vi = 0;
  this->vertices[vi++] = O + Vector3D(-dims[0] / 2.0f, +dims[1] / 2.0f, 0.0f);
  this->vertices[vi++] = O + Vector3D(-dims[0] / 2.0f, -dims[1] / 2.0f, 0.0f);
  this->vertices[vi++] = O + Vector3D(+dims[0] / 2.0f, -dims[1] / 2.0f, 0.0f);
  this->vertices[vi++] = O + Vector3D(+dims[0] / 2.0f, +dims[1] / 2.0f, 0.0f);

  for (int i=0;i<this->num_vert;i++){
    this->vertices[i].RotateArbitraryAxis(O,axis,degree);
    this->bBox.addPoint(this->vertices[i]);
  }

	trisN = 0;
	int tri = 0;

	this->triangles[tri * 3 + 0] = 0;
	this->triangles[tri * 3 + 1] = 1;
	this->triangles[tri * 3 + 2] = 2;
	tri++;
	this->triangles[tri * 3 + 0] = 2;
	this->triangles[tri * 3 + 1] = 3;
	this->triangles[tri * 3 + 2] = 0;
	tri++;
	trisN += 2;

  this->com = O;

  Vector3D n = (this->vertices[0] - this->vertices[1])\
                .CrossProduct(this->vertices[1] - this->vertices[2])\
                .Normalize();
  for (int i = 0; i< this->num_vert;i++){
    this->normals[i] = n;
  }
}

void TriangleMesh::RenderTextureQuad(PPC *ppc, FrameBuffer *fb, Texture *texture, float scale)
{
  float width = fb->getwidth();
  float height = fb->getheight();

  if (this->quad != QUAD)
    return;

  Vector3D pvs[4];  // projected point
  Vector3D vs[4];   // point
  Vector3D st[4];   // barycentric coordinates
  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
  for (int vi = 0; vi < this->num_vert; vi++) {
    if (!ppc->project(this->vertices[vi], projV[vi]))
      projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
  }

  /* get projected triangles */
  vs[0] = this->vertices[0]; vs[1] = this->vertices[1];
  vs[2] = this->vertices[2]; vs[3] = this->vertices[3];
  pvs[0] = projV[0]; pvs[1] = projV[1];
  pvs[2] = projV[2]; pvs[3] = projV[3];
  // texel coor
  st[0] = Vector3D(0.0f,0.0f,0.0f); st[1] = Vector3D(0.0f,scale,0.0f);
  st[2] = Vector3D(scale,scale,0.0f); st[3] = Vector3D(scale,0.0f,0.0f);
  if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX || pvs[3][0] == FLT_MAX)
    return;

  // bounding box
  float max_x = -FLT_MAX;
  float min_x = FLT_MAX;
  float max_y = -FLT_MAX;
  float min_y = FLT_MAX;
  for (int i=0;i<4;i++){
    if (pvs[i][0] > max_x) max_x = pvs[i][0];
    if (pvs[i][0] < min_x) min_x = pvs[i][0];
    if (pvs[i][1] > max_y) max_y = pvs[i][1];
    if (pvs[i][1] < min_y) min_y = pvs[i][1];
  }
  if (max_x > width) max_x = width;
  if (min_x < 0) min_x = 0;
  if (max_y > height) max_y = height;
  if (min_y < 0) min_y = 0;
  int left = (int) (min_x+0.5f), right = (int) (max_x-0.5f);
  int top = (int) (min_y+0.5f), bottom = (int) (max_y-0.5f);

  // calculate bary coordinates
  Matrix3D cart2bary1, cart2bary2;
  cart2bary1.SetColumn(vs[0],0);
  cart2bary1.SetColumn(vs[1],1);
  cart2bary1.SetColumn(vs[2],2);
  try{
    cart2bary1 = cart2bary1.Inverse();
  }
  catch (const char *msg){
    printf("%s\n",msg);
  }
  cart2bary2.SetColumn(vs[2],0);
  cart2bary2.SetColumn(vs[3],1);
  cart2bary2.SetColumn(vs[0],2);
  try{
    cart2bary2 = cart2bary2.Inverse();
  }
  catch (const char *msg){
    printf("%s\n",msg);
  }

  // sidedness
  Vector3D eqs1[3], eqs2[3];
  eqs1[0] = this->setSidednessEQ(pvs[0], pvs[1], pvs[2]);
  eqs1[1] = this->setSidednessEQ(pvs[2], pvs[0], pvs[1]);
  eqs1[2] = this->setSidednessEQ(pvs[1], pvs[2], pvs[0]);
  eqs2[0] = this->setSidednessEQ(pvs[2], pvs[3], pvs[0]);
  eqs2[1] = this->setSidednessEQ(pvs[0], pvs[2], pvs[3]);
  eqs2[2] = this->setSidednessEQ(pvs[3], pvs[0], pvs[2]);
  // normal of triangle
  Vector3D normal = (vs[1]-vs[0]).CrossProduct(vs[2]-vs[1]).Normalize();

  // set color
  int curr_x, curr_y;
  int dimension = max(right-left,bottom-top);
  for (curr_y = top; curr_y <= bottom; curr_y++){
    for (curr_x = left; curr_x <= right; curr_x++){
      Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
      //Vector3D proj = ppc->c + ppc->a * ((float)curr_x + .5f) + ppc->b * ((float)curr_y + .5f);
      Vector3D proj = ppc->c + ppc->a * currPix[0] + ppc->b * currPix[1];
      unsigned int color;

      if (currPix * eqs1[0] >= 0.0f && currPix * eqs1[1] >= 0.0f
        && currPix * eqs1[2] >= 0.0f){
        float lambda = (float) (normal * (vs[1] - ppc->C)) / (normal * proj);
        // bary coordinates
        Vector3D bary_coor = cart2bary1 * (ppc->C + proj * lambda);
        // color
        Vector3D st_coor = st[0]*bary_coor[0] + st[1]*bary_coor[1] + st[2]*bary_coor[2];
        color = texture->getPixel(fmod(st_coor[0],scale),fmod(st_coor[1],scale));
        currPix[2] = 1.0f / ((1.0f / pvs[0][2])*bary_coor[0] + (1.0f / pvs[1][2])*bary_coor[1] \
                    + (1.0f / pvs[2][2])*bary_coor[2]);
        fb->setzBuffer(currPix, color);
      }
      else if(currPix * eqs2[0] >= 0.0f && currPix * eqs2[1] >= 0.0f
        && currPix * eqs2[2] >= 0.0f){
        float lambda = (float) (normal * (vs[3] - ppc->C)) / (normal * proj);
        // bary coordinates
        Vector3D bary_coor = cart2bary2 * (ppc->C + proj * lambda);
        // color
        Vector3D st_coor = st[2]*bary_coor[0] + st[3]*bary_coor[1] + st[0]*bary_coor[2];
        color = texture->getPixel(fmod(st_coor[0],scale),fmod(st_coor[1],scale));
        currPix[2] = 1.0f / ((1.0f / pvs[2][2])*bary_coor[0] + (1.0f / pvs[3][2])*bary_coor[1] \
                    + (1.0f / pvs[0][2])*bary_coor[2]);
        fb->setzBuffer(currPix, color);
      }
    }
  }

  delete [] projV;
}


void TriangleMesh::RenderTextureQuad(PPC *ppc, FrameBuffer *fb, Mipmap *mipmap, float scale)
{
  float width = fb->getwidth();
  float height = fb->getheight();

  if (this->quad != QUAD)
    return;

  Vector3D pvs[4];  // projected point
  Vector3D vs[4];   // point
  Vector3D st[4];   // barycentric coordinates
  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
  for (int vi = 0; vi < this->num_vert; vi++) {
    if (!ppc->project(this->vertices[vi], projV[vi]))
      projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
  }

  /* get projected triangles */
  vs[0] = this->vertices[0]; vs[1] = this->vertices[1];
  vs[2] = this->vertices[2]; vs[3] = this->vertices[3];
  pvs[0] = projV[0]; pvs[1] = projV[1];
  pvs[2] = projV[2]; pvs[3] = projV[3];
  // texel coor
  st[0] = Vector3D(0.0f,0.0f,0.0f); st[1] = Vector3D(0.0f,scale,0.0f);
  st[2] = Vector3D(scale,scale,0.0f); st[3] = Vector3D(scale,0.0f,0.0f);
  if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX || pvs[3][0] == FLT_MAX)
    return;

  // bounding box
  float max_x = -FLT_MAX;
  float min_x = FLT_MAX;
  float max_y = -FLT_MAX;
  float min_y = FLT_MAX;
  for (int i=0;i<4;i++){
    if (pvs[i][0] > max_x) max_x = pvs[i][0];
    if (pvs[i][0] < min_x) min_x = pvs[i][0];
    if (pvs[i][1] > max_y) max_y = pvs[i][1];
    if (pvs[i][1] < min_y) min_y = pvs[i][1];
  }
  if (max_x > width) max_x = width;
  if (min_x < 0) min_x = 0;
  if (max_y > height) max_y = height;
  if (min_y < 0) min_y = 0;
  int left = (int) (min_x+0.5f), right = (int) (max_x-0.5f);
  int top = (int) (min_y+0.5f), bottom = (int) (max_y-0.5f);

  // calculate bary coordinates
  Matrix3D cart2bary1, cart2bary2;
  cart2bary1.SetColumn(vs[0],0);
  cart2bary1.SetColumn(vs[1],1);
  cart2bary1.SetColumn(vs[2],2);
  try{
    cart2bary1 = cart2bary1.Inverse();
  }
  catch (const char *msg){
    printf("%s\n",msg);
  }
  cart2bary2.SetColumn(vs[2],0);
  cart2bary2.SetColumn(vs[3],1);
  cart2bary2.SetColumn(vs[0],2);
  try{
    cart2bary2 = cart2bary2.Inverse();
  }
  catch (const char *msg){
    printf("%s\n",msg);
  }

  // sidedness
  Vector3D eqs1[3], eqs2[3];
  eqs1[0] = this->setSidednessEQ(pvs[0], pvs[1], pvs[2]);
  eqs1[1] = this->setSidednessEQ(pvs[2], pvs[0], pvs[1]);
  eqs1[2] = this->setSidednessEQ(pvs[1], pvs[2], pvs[0]);
  eqs2[0] = this->setSidednessEQ(pvs[2], pvs[3], pvs[0]);
  eqs2[1] = this->setSidednessEQ(pvs[0], pvs[2], pvs[3]);
  eqs2[2] = this->setSidednessEQ(pvs[3], pvs[0], pvs[2]);
  // normal of triangle
  Vector3D normal = (vs[1]-vs[0]).CrossProduct(vs[2]-vs[1]).Normalize();

  // set color
  int curr_x, curr_y;
  int dimension = max(right-left,bottom-top);
  for (curr_y = top; curr_y <= bottom; curr_y++){
    for (curr_x = left; curr_x <= right; curr_x++){
      Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
      //Vector3D proj = ppc->c + ppc->a * ((float)curr_x + .5f) + ppc->b * ((float)curr_y + .5f);
      Vector3D proj = ppc->c + ppc->a * currPix[0] + ppc->b * currPix[1];
      unsigned int color;

      if (currPix * eqs1[0] >= 0.0f && currPix * eqs1[1] >= 0.0f
        && currPix * eqs1[2] >= 0.0f){
        float lambda = (float) (normal * (vs[1] - ppc->C)) / (normal * proj);
        // bary coordinates
        Vector3D bary_coor = cart2bary1 * (ppc->C + proj * lambda);
        // color
        Vector3D st_coor = st[0]*bary_coor[0] + st[1]*bary_coor[1] + st[2]*bary_coor[2];
        color = mipmap->getPixel(fmod(st_coor[0],scale),fmod(st_coor[1],scale),dimension);
        currPix[2] = 1.0f / ((1.0f / pvs[0][2])*bary_coor[0] + (1.0f / pvs[1][2])*bary_coor[1] \
                    + (1.0f / pvs[2][2])*bary_coor[2]);
        fb->setzBuffer(currPix, color);
      }
      else if(currPix * eqs2[0] >= 0.0f && currPix * eqs2[1] >= 0.0f
        && currPix * eqs2[2] >= 0.0f){
        float lambda = (float) (normal * (vs[3] - ppc->C)) / (normal * proj);
        // bary coordinates
        Vector3D bary_coor = cart2bary2 * (ppc->C + proj * lambda);
        // color
        Vector3D st_coor = st[2]*bary_coor[0] + st[3]*bary_coor[1] + st[0]*bary_coor[2];
        color = mipmap->getPixel(fmod(st_coor[0],scale),fmod(st_coor[1],scale),dimension);
        currPix[2] = 1.0f / ((1.0f / pvs[2][2])*bary_coor[0] + (1.0f / pvs[3][2])*bary_coor[1] \
                    + (1.0f / pvs[0][2])*bary_coor[2]);
        fb->setzBuffer(currPix, color);
      }
    }
  }

  delete [] projV;
}

void TriangleMesh::buildShadowMap(PSL *psl)
{

  float x0, y0, x1, y1, x2, y2;
  float width = psl->getWidth();
  float height = psl->getHeight();

  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
	for (int vi = 0; vi < this->num_vert; vi++) {
		if (!psl->project(this->vertices[vi], projV[vi]))
			projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	}

  for (int tri = 0;tri < this->num_tri;tri++){
    /* get projected triangles */
    Vector3D pvs[3];
    Vector3D col[3];
    Vector3D vs[3];
    vs[0] = this->vertices[this->triangles[3 * tri + 0]];
		vs[1] = this->vertices[this->triangles[3 * tri + 1]];
		vs[2] = this->vertices[this->triangles[3 * tri + 2]];
		pvs[0] = projV[this->triangles[3 * tri + 0]];
		pvs[1] = projV[this->triangles[3 * tri + 1]];
		pvs[2] = projV[this->triangles[3 * tri + 2]];
    col[0] = this->cols[this->triangles[3 * tri + 0]];
    col[1] = this->cols[this->triangles[3 * tri + 1]];
    col[2] = this->cols[this->triangles[3 * tri + 2]];
		if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX)
			continue;
    Vector3D eqs[3];
    for (int i = 0; i < 3; i++) {
			eqs[i] = this->setSidednessEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
		}

    x0 = pvs[0][0]; y0 = pvs[0][1];
    x1 = pvs[1][0]; y1 = pvs[1][1];
    x2 = pvs[2][0]; y2 = pvs[2][1];
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

    // normal of triangle
    Vector3D normal = (vs[1]-vs[0]).CrossProduct(vs[2]-vs[1]).Normalize();

    // for model space color interpolation
    Matrix3D cart2bary;
    cart2bary.SetColumn(Vector3D(pvs[0][0], pvs[1][0], pvs[2][0]),0);
		cart2bary.SetColumn(Vector3D(pvs[0][1], pvs[1][1], pvs[2][1]),1);
		cart2bary.SetColumn(Vector3D(1.0f, 1.0f, 1.0f),2);
    try{
      cart2bary = cart2bary.Inverse();
    }
    catch (const char *msg){
      printf("%s\n",msg);
    }
    Vector3D zABC = cart2bary*Vector3D(pvs[0][2], pvs[1][2], pvs[2][2]);

    // set zbuffer
    int curr_x, curr_y;
    for (curr_y = top; curr_y <= bottom; curr_y++){
      for (curr_x = left; curr_x <= right; curr_x++){
        Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
				if (currPix * eqs[0] < 0.0f || currPix * eqs[1] < 0.0f
					|| currPix * eqs[2] < 0.0f)
					continue;
        currPix[2] = currPix*zABC;
				psl->setzBuffer(currPix);
      }
    }
  }

  delete [] projV;

}

void TriangleMesh::buildProjTexMap(TP *tp)
{
  float x0, y0, x1, y1, x2, y2;
  PPC *ppc = tp->ppc;
  float width = ppc->getWidth();
  float height = ppc->getHeight();


  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
  for (int vi = 0; vi < this->num_vert; vi++) {
    if (!ppc->project(this->vertices[vi], projV[vi]))
      projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
  }

  for (int tri = 0;tri < this->num_tri;tri++){
    /* get projected triangles */
    Vector3D pvs[3];
    Vector3D col[3];
    Vector3D vs[3];
    vs[0] = this->vertices[this->triangles[3 * tri + 0]];
    vs[1] = this->vertices[this->triangles[3 * tri + 1]];
    vs[2] = this->vertices[this->triangles[3 * tri + 2]];
    pvs[0] = projV[this->triangles[3 * tri + 0]];
    pvs[1] = projV[this->triangles[3 * tri + 1]];
    pvs[2] = projV[this->triangles[3 * tri + 2]];
    col[0] = this->cols[this->triangles[3 * tri + 0]];
    col[1] = this->cols[this->triangles[3 * tri + 1]];
    col[2] = this->cols[this->triangles[3 * tri + 2]];
    if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX)
      continue;
    Vector3D eqs[3];
    for (int i = 0; i < 3; i++) {
      eqs[i] = this->setSidednessEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
    }

    x0 = pvs[0][0]; y0 = pvs[0][1];
    x1 = pvs[1][0]; y1 = pvs[1][1];
    x2 = pvs[2][0]; y2 = pvs[2][1];
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

    // normal of triangle
    Vector3D normal = (vs[1]-vs[0]).CrossProduct(vs[2]-vs[1]).Normalize();

    // for model space color interpolation
    Matrix3D cart2bary;
    cart2bary.SetColumn(vs[0],0);
    cart2bary.SetColumn(vs[1],1);
    cart2bary.SetColumn(vs[2],2);
    try{
      cart2bary = cart2bary.Inverse();
    }
    catch (const char *msg){
      printf("%s\n",msg);
    }

    // set zbuffer
    int curr_x, curr_y;
    for (curr_y = top; curr_y <= bottom; curr_y++){
      for (curr_x = left; curr_x <= right; curr_x++){
        Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
        if (currPix * eqs[0] < 0.0f || currPix * eqs[1] < 0.0f
          || currPix * eqs[2] < 0.0f)
          continue;
        Vector3D proj = ppc->getc() + ppc->geta() * currPix[0] + ppc->getb() * currPix[1];
        float lambda = (float) (normal * (vs[1] - ppc->getC())) / (normal * proj);
        tp->setzBuffer(ppc->getC() + proj * lambda);
      }
    }
  }

  delete [] projV;
}

void TriangleMesh::RenderProjMap(PPC *ppc, FrameBuffer *fb, TP *tp)
{
  float x0, y0, x1, y1, x2, y2;
  float width = fb->getwidth();
  float height = fb->getheight();

  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
	for (int vi = 0; vi < this->num_vert; vi++) {
		if (!ppc->project(this->vertices[vi], projV[vi]))
			projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	}

  for (int tri=0;tri<this->num_tri;tri++){
    /* get projected triangles */
    Vector3D pvs[3];
    Vector3D col[3];
    Vector3D vs[3];
    vs[0] = this->vertices[this->triangles[3 * tri + 0]];
		vs[1] = this->vertices[this->triangles[3 * tri + 1]];
		vs[2] = this->vertices[this->triangles[3 * tri + 2]];
		pvs[0] = projV[this->triangles[3 * tri + 0]];
		pvs[1] = projV[this->triangles[3 * tri + 1]];
		pvs[2] = projV[this->triangles[3 * tri + 2]];
    col[0] = this->cols[this->triangles[3 * tri + 0]];
    col[1] = this->cols[this->triangles[3 * tri + 1]];
    col[2] = this->cols[this->triangles[3 * tri + 2]];
		if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX)
			continue;
    Vector3D eqs[3];
    for (int i = 0; i < 3; i++) {
			eqs[i] = this->setSidednessEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
		}

    x0 = pvs[0][0]; y0 = pvs[0][1];
    x1 = pvs[1][0]; y1 = pvs[1][1];
    x2 = pvs[2][0]; y2 = pvs[2][1];
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

    // normal of triangle
    Vector3D normal = ((vs[1]-vs[0]).CrossProduct(vs[2]-vs[1])).Normalize();

    // for model space color interpolation
    Matrix3D cart2bary;
    cart2bary.SetColumn(vs[0],0);
		cart2bary.SetColumn(vs[1],1);
		cart2bary.SetColumn(vs[2],2);
    try{
		  cart2bary = cart2bary.Inverse();
    }
    catch (const char *msg){
      continue;
    }

    // set color
    int curr_x, curr_y;
    for (curr_y = top; curr_y <= bottom; curr_y++){
      for (curr_x = left; curr_x <= right; curr_x++){
        Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
				if (currPix * eqs[0] < 0.0f || currPix * eqs[1] < 0.0f
					|| currPix * eqs[2] < 0.0f)
					continue;
        Vector3D proj = ppc->c + ppc->a * currPix[0] + ppc->b * currPix[1];
        float lambda = (float) (normal * (vs[0] - ppc->C)) / (normal * proj);
        Vector3D bary_coor = (cart2bary * (ppc->C + proj * lambda));
        //Vector3D cv = col[0]*bary_coor[0] + col[1]*bary_coor[1] + col[2]*bary_coor[2];
        Vector3D cv = (col[0]*bary_coor[0]+col[1]*bary_coor[1]+col[2]*bary_coor[2]);
				currPix[2] = 1.0f / ((1.0f / pvs[0][2])*bary_coor[0] + (1.0f / pvs[1][2])*bary_coor[1] \
                    + (1.0f / pvs[2][2])*bary_coor[2]);
        unsigned int ret = tp->renderOrNot(ppc->C + proj * lambda);
        //cout << std::hex << ret << endl;
        if (!ret)
          fb->setzBuffer(currPix, fb->getColorFromVector(cv));
        else
				  fb->setzBuffer(currPix, ret);
      }
    }
  }

  delete [] projV;
}

void TriangleMesh::RenderReflectionMap(PPC *ppc, FrameBuffer *fb, CubeMap *cubemap)
{
  float x0, y0, x1, y1, x2, y2;
  float width = fb->getwidth();
  float height = fb->getheight();
  float u, v;
  int image_idx;

  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
	for (int vi = 0; vi < this->num_vert; vi++) {
		if (!ppc->project(this->vertices[vi], projV[vi]))
			projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	}

  for (int tri=0;tri<this->num_tri;tri++){
    /* get projected triangles */
    Vector3D pvs[3];
    Matrix3D vs;
    Matrix3D normal;
    vs[0] = this->vertices[this->triangles[3 * tri + 0]];
		vs[1] = this->vertices[this->triangles[3 * tri + 1]];
		vs[2] = this->vertices[this->triangles[3 * tri + 2]];
		pvs[0] = projV[this->triangles[3 * tri + 0]];
		pvs[1] = projV[this->triangles[3 * tri + 1]];
		pvs[2] = projV[this->triangles[3 * tri + 2]];
    if (this->normals){
      normal[0] = this->normals[this->triangles[3 * tri + 0]];
      normal[1] = this->normals[this->triangles[3 * tri + 1]];
      normal[2] = this->normals[this->triangles[3 * tri + 2]];
    }
		if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX)
			continue;
    Vector3D eqs[3];
    for (int i = 0; i < 3; i++) {
			eqs[i] = this->setSidednessEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
		}

    x0 = pvs[0][0]; y0 = pvs[0][1];
    x1 = pvs[1][0]; y1 = pvs[1][1];
    x2 = pvs[2][0]; y2 = pvs[2][1];
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
    Matrix3D ISI;
    Matrix3D MSI;
    try{
      ISI = computeSSIM(pvs);
      MSI = computeMSIM(ppc, vs);
    }
    catch (const char *msg){
      continue;
    }
    Vector3D denABC = MSI[0] + MSI[1] + MSI[2];
    Matrix3D nmsNumABC = normal.Transpose()*MSI;
    Vector3D zABC = ISI*Vector3D(pvs[0][2], pvs[1][2], pvs[2][2]);

    for (curr_y = top; curr_y <= bottom; curr_y++){
      for (curr_x = left; curr_x <= right; curr_x++){
        Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
				if (currPix * eqs[0] < 0.0f || currPix * eqs[1] < 0.0f
					|| currPix * eqs[2] < 0.0f)
          continue;

        Vector3D nmsv = (nmsNumABC*currPix) / (denABC*currPix);
				nmsv = nmsv.Normalize();
        Vector3D proj = ppc->c + ppc->a * currPix[0] + ppc->b * currPix[1];
        Vector3D refle = proj - nmsv * (nmsv * proj) * 2.0f;
        currPix[2] = currPix*zABC;

        unsigned int pixel;
        if(cubemap){
          cubemap->xyz2uv(refle,&image_idx,&u,&v);
          pixel = cubemap->getPixel(image_idx,u,v);
        }
        else{
          pixel = 0x000000FF;
        }

  			fb->setzBuffer(currPix, pixel);
      }
    }
  }

  delete [] projV;
}

Matrix3D TriangleMesh::computeSSIM(Vector3D *pvs) {

	Matrix3D ssim;
	ssim.SetColumn(Vector3D(pvs[0][0], pvs[1][0], pvs[2][0]), 0);
	ssim.SetColumn(Vector3D(pvs[0][1], pvs[1][1], pvs[2][1]), 1);
	ssim.SetColumn(Vector3D(1.0f, 1.0f, 1.0f), 2);
	ssim = ssim.Inverse();
	return ssim;

}

Matrix3D TriangleMesh::computeMSIM(PPC *ppc, Matrix3D vs)
{
  Matrix3D qm = vs.Transpose();
  Vector3D C = ppc->getC();
	qm.SetColumn(qm.GetColumn(0) - C, 0);
	qm.SetColumn(qm.GetColumn(1) - C, 1);
	qm.SetColumn(qm.GetColumn(2) - C, 2);

	Matrix3D cam;
	cam.SetColumn(ppc->a, 0);
	cam.SetColumn(ppc->b, 1);
	cam.SetColumn(ppc->c, 2);

	qm = qm.Inverse() * cam;

	return qm;
}

void TriangleMesh::RenderRefractionMap(PPC *ppc, FrameBuffer *fb, CubeMap *cubemap)
{
  float x0, y0, x1, y1, x2, y2;
  float width = fb->getwidth();
  float height = fb->getheight();
  float u, v;
  int image_idx;
  float refra_idx = 1.0f / 1.3f;  // refraction index

  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
	for (int vi = 0; vi < this->num_vert; vi++) {
		if (!ppc->project(this->vertices[vi], projV[vi]))
			projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	}

  for (int tri=0;tri<this->num_tri;tri++){
    /* get projected triangles */
    Vector3D pvs[3];
    Matrix3D vs;
    Matrix3D normal;
    vs[0] = this->vertices[this->triangles[3 * tri + 0]];
		vs[1] = this->vertices[this->triangles[3 * tri + 1]];
		vs[2] = this->vertices[this->triangles[3 * tri + 2]];
		pvs[0] = projV[this->triangles[3 * tri + 0]];
		pvs[1] = projV[this->triangles[3 * tri + 1]];
		pvs[2] = projV[this->triangles[3 * tri + 2]];
    if (this->normals){
      normal[0] = this->normals[this->triangles[3 * tri + 0]];
      normal[1] = this->normals[this->triangles[3 * tri + 1]];
      normal[2] = this->normals[this->triangles[3 * tri + 2]];
    }
		if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX)
			continue;
    Vector3D eqs[3];
    for (int i = 0; i < 3; i++) {
			eqs[i] = this->setSidednessEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
		}

    x0 = pvs[0][0]; y0 = pvs[0][1];
    x1 = pvs[1][0]; y1 = pvs[1][1];
    x2 = pvs[2][0]; y2 = pvs[2][1];
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
    Matrix3D ISI;
    Matrix3D MSI;
    try{
      ISI = computeSSIM(pvs);
      MSI = computeMSIM(ppc, vs);
    }
    catch (const char *msg){
      continue;
    }
    Vector3D denABC = MSI[0] + MSI[1] + MSI[2];
    Matrix3D nmsNumABC = normal.Transpose()*MSI;
    Vector3D zABC = ISI*Vector3D(pvs[0][2], pvs[1][2], pvs[2][2]);


    for (curr_y = top; curr_y <= bottom; curr_y++){
      for (curr_x = left; curr_x <= right; curr_x++){
        Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
				if (currPix * eqs[0] < 0.0f || currPix * eqs[1] < 0.0f
					|| currPix * eqs[2] < 0.0f)
					continue;

        Vector3D nmsv = (nmsNumABC*currPix) / (denABC*currPix);
        //Vector3D nmsv(0.0f,0.0f,1.0f);
				nmsv = nmsv.Normalize();
        Vector3D proj = ppc->c + ppc->a * currPix[0] + ppc->b * currPix[1];

        Vector3D refra = (proj - nmsv * (nmsv * proj)) * refra_idx + nmsv * (nmsv * proj);
        currPix[2] = currPix*zABC;
        cubemap->xyz2uv(refra,&image_idx,&u,&v);
        unsigned int pixel = cubemap->getPixel(image_idx,u,v);

  			fb->setzBuffer(currPix, pixel);
      }
    }
  }

  delete [] projV;
}

void TriangleMesh::RenderBillBoard(PPC *ppc, FrameBuffer *fb, PPC *bbppc, FrameBuffer *bb, CubeMap *cubemap)
{
  float x0, y0, x1, y1, x2, y2;
  float width = fb->getwidth();
  float height = fb->getheight();
  float u, v;
  int image_idx;

  /* project all vertices */
  Vector3D *projV = new Vector3D[this->num_vert];
  for (int vi = 0; vi < this->num_vert; vi++) {
    if (!ppc->project(this->vertices[vi], projV[vi]))
      projV[vi] = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
  }

  Vector3D bbproj;
  unsigned int bbppc_width = bbppc->getWidth();
  unsigned int bbppc_height = bbppc->getHeight();
  for (int tri=0;tri<this->num_tri;tri++){
    /* get projected triangles */
    Vector3D pvs[3];
    Matrix3D vs;
    Matrix3D normal;
    vs[0] = this->vertices[this->triangles[3 * tri + 0]];
    vs[1] = this->vertices[this->triangles[3 * tri + 1]];
    vs[2] = this->vertices[this->triangles[3 * tri + 2]];
    pvs[0] = projV[this->triangles[3 * tri + 0]];
    pvs[1] = projV[this->triangles[3 * tri + 1]];
    pvs[2] = projV[this->triangles[3 * tri + 2]];
    if (this->normals){
      normal[0] = this->normals[this->triangles[3 * tri + 0]];
      normal[1] = this->normals[this->triangles[3 * tri + 1]];
      normal[2] = this->normals[this->triangles[3 * tri + 2]];
    }
    if (pvs[0][0] == FLT_MAX || pvs[1][0] == FLT_MAX || pvs[2][0] == FLT_MAX)
      continue;
    Vector3D eqs[3];
    for (int i = 0; i < 3; i++) {
      eqs[i] = this->setSidednessEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
    }

    x0 = pvs[0][0]; y0 = pvs[0][1];
    x1 = pvs[1][0]; y1 = pvs[1][1];
    x2 = pvs[2][0]; y2 = pvs[2][1];
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
    Matrix3D ISI;
    Matrix3D MSI;
    try{
      ISI = computeSSIM(pvs);
      MSI = computeMSIM(ppc, vs);
    }
    catch (const char *msg){
      continue;
    }
    Vector3D denABC = MSI[0] + MSI[1] + MSI[2];
    Matrix3D nmsNumABC = normal.Transpose()*MSI;
    Vector3D zABC = ISI*Vector3D(pvs[0][2], pvs[1][2], pvs[2][2]);


    for (curr_y = top; curr_y <= bottom; curr_y++){
      for (curr_x = left; curr_x <= right; curr_x++){
        Vector3D currPix(.5f + (float)curr_x, .5f + (float)curr_y, 1.0f);
        if (currPix * eqs[0] < 0.0f || currPix * eqs[1] < 0.0f
          || currPix * eqs[2] < 0.0f)
          continue;

        Vector3D nmsv = (nmsNumABC*currPix) / (denABC*currPix);
        nmsv = nmsv.Normalize();
        Vector3D proj = ppc->c + ppc->a * currPix[0] + ppc->b * currPix[1];
        Vector3D refle = proj - nmsv * (nmsv * proj) * 2.0f;
        currPix[2] = currPix*zABC;

        bbppc->project(refle,bbproj);
        // cubemap
        if (bbproj[0] < 0 || bbproj[0] >= bbppc_width || bbproj[1] < 0 || bbproj[1] >= bbppc_height){
          cubemap->xyz2uv(refle,&image_idx,&u,&v);
          unsigned int pixel = cubemap->getPixel(image_idx,u,v);
          fb->setzBuffer(currPix, pixel);
        }
        // billboard
        else{
          fb->setzBuffer(currPix, bb->get(bbproj[0],bbproj[1]));
        }

      }
    }
  }

  delete [] projV;
}

void TriangleMesh::RenderHW(unsigned int mode) {

  /* filled mode */

  if (mode == FILLED)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  /* wireframe mode */
  else if (mode == WIREFRAME)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (float*)this->vertices);
	glColorPointer(3, GL_FLOAT, 0, (float*)this->cols);
  glNormalPointer(GL_FLOAT, 0, (float*)this->normals);
	glDrawElements(GL_TRIANGLES, 3 * this->num_tri, GL_UNSIGNED_INT, this->triangles);
  glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);


}
