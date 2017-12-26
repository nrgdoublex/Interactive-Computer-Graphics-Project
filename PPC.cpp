#include "PPC.h"
//#define DEBUG_PPC

using namespace std;

/*-----------------------------------------------------
  Constructor
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  unsigned int w:     width of PPC
  unsigned int h:     height of PPC
  float fofv:         field of view
Return:
-----------------------------------------------------*/
PPC::PPC(unsigned int w, unsigned int h, float fofv)
{
  this->width = w;
  this->height = h;
  this->C = Vector3D(0.0f, 0.0f, 0.0f);
	this->a = Vector3D(1.0f, 0.0f, 0.0f);
	this->b = Vector3D(0.0f, -1.0f, 0.0f);
	float h2 = fofv*PI / 180.0f / 2.0f;
  this->flength = -(float)w / (2.0f*tanf(h2));
	this->c = Vector3D(-0.5f*(float)w, 0.5f*(float)h, this->flength);

#if defined DEBUG_PPC
  cout << getFocal() << endl;
  Vector3D vd = this->a.CrossProduct(this->b);
  cout << vd*this->c << endl;
#endif
}

/*-----------------------------------------------------
  Get view direction
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
Return:
  Vector3D: view direction vector
-----------------------------------------------------*/
Vector3D PPC::getVD()
{
  return this->a.CrossProduct(this->b).Normalize();
}

/*-----------------------------------------------------
  Get C vector of PPC
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
  Vector3D: C vector of PPC
-----------------------------------------------------*/
Vector3D PPC::getC()
{
  return this->C;
}

/*-----------------------------------------------------
  Set C vector of PPC
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D newC:    new C vector
Return:
-----------------------------------------------------*/
void PPC::setC(Vector3D newC)
{
  this->C.Set(newC);
}

Vector3D PPC::geta()
{
  return this->a;
}

void PPC::seta(Vector3D newa)
{
  this->a.Set(newa);
}

Vector3D PPC::getb()
{
  return this->b;
}

void PPC::setb(Vector3D newb)
{
  this->b.Set(newb);
}

/*-----------------------------------------------------
  Set c vector of PPC
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
Return:
  Vector3D: c vector of PPC
-----------------------------------------------------*/
Vector3D PPC::getc()
{
  return this->c;
}

void PPC::setc(Vector3D newc)
{
  this->c.Set(newc);
}

/*-----------------------------------------------------
  Get width of PPC
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
Return:
  float: width of PPC
-----------------------------------------------------*/
float PPC::getWidth()
{
  return this->width;
}

/*-----------------------------------------------------
  Get height of PPC
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
Return:
  float: height of PPC
-----------------------------------------------------*/
float PPC::getHeight()
{
  return this->height;
}

/*-----------------------------------------------------
  Move the camera by a given vector
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D translation: vector by which to translate mesh
Return:
-----------------------------------------------------*/
void PPC::translate(Vector3D translation)
{
  this->C.Set(this->C + translation);
}

/*-----------------------------------------------------
  Move the camera along a vector
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  float steps: amounts to move along a vector
Return:
-----------------------------------------------------*/
void PPC::leftrightTranslate(float steps)
{
  this->C.Set(this->C + (this->a.Normalize()*steps));
}

/*-----------------------------------------------------
  Move the camera along b vector
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  float steps: amounts to move along b vector
Return:
-----------------------------------------------------*/
void PPC::updownTranslate(float steps)
{
  this->C.Set(this->C + (this->b.Normalize()*steps));
}

/*-----------------------------------------------------
  Move the camera along axb vector
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  float steps: amounts to move along axb vector
Return:
-----------------------------------------------------*/
void PPC::forwardbackwardTranslate(float steps)
{
  Vector3D normal = this->a.CrossProduct(this->b).Normalize();
  this->C.Set(this->C + (normal*steps));
}

/*-----------------------------------------------------
  Pan Rotation
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  float steps: amounts to pan
Return:
-----------------------------------------------------*/
void PPC::pan(float steps)
{
  this->c.RotateArbitraryDirection(this->b*(-1),steps);
  this->a.RotateArbitraryDirection(this->b*(-1),steps);

#if defined DEBUG_PPC
  cout << getFocal() << endl;
#endif
}

/*-----------------------------------------------------
  Tilt rotation
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  float steps: amounts to tile
Return:
-----------------------------------------------------*/
void PPC::tilt(float steps)
{
  this->c.RotateArbitraryDirection(this->a,steps);
  this->b.RotateArbitraryDirection(this->a,steps);

#if defined DEBUG_PPC
  cout << getFocal() << endl;
#endif
}

/*-----------------------------------------------------
  Row rotation
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  float steps: amounts to row
Return:
-----------------------------------------------------*/
void PPC::row(float steps)
{
  Vector3D normal = this->getVD()*(-1);
  this->a.RotateArbitraryDirection(normal,steps);
  this->b.RotateArbitraryDirection(normal,steps);
  this->c.RotateArbitraryDirection(normal,steps);

#if defined DEBUG_PPC
  cout << getFocal() << endl;
#endif
}

/*-----------------------------------------------------
  Update focal fength
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  float len: new focal length
Return:
-----------------------------------------------------*/
void PPC::setFocal(float len)
{
  // find view direction
  float ppu = 0.5f*(float)this->width;
  float ppv = 0.5f*(float)this->height;
  Vector3D vd = (this->c + this->a*ppu + this->b*ppv);
  cout << vd.Length() << endl;
  vd = vd.Normalize();
  Vector3D newc = this->a*(-ppu) + this->b*(-ppv) + vd*len;
  this->c.Set(newc);
}

/*-----------------------------------------------------
  Get focal fength
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:

Return:
  float: current focal length
-----------------------------------------------------*/
float PPC::getFocal()
{
  return this->getVD()*this->c;
}

/*-----------------------------------------------------
  Project 3D points
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D point: 3D point to project
  Vector3D &project: projected point
Return:
  int: always return 1
-----------------------------------------------------*/
int PPC::project(Vector3D point, Vector3D &project)
{
  Matrix3D cam;
	cam.SetColumn(this->a, 0);
	cam.SetColumn(this->b, 1);
	cam.SetColumn(this->c, 2);

	Vector3D q = cam.Inverse()*(point - this->C);

	if (q[2] <= 0.0f)
		return 0;

	project = q / q[2];
  project[2] = 1.0f / q[2];
	return 1;
}

/*-----------------------------------------------------
  Save PPC parameters to file
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  const char *filepath: file path to save
Return:
  int: 0 for success, -1 otherwise
-----------------------------------------------------*/
int PPC::save(const char *filepath)
{
  FILE *fh = fopen(filepath,"w");
  if (fh){
    fprintf(fh,"a Vector: (%f,%f,%f)\n",this->a[0],this->a[1],this->a[2]);
    fprintf(fh,"b Vector: (%f,%f,%f)\n",this->b[0],this->b[1],this->b[2]);
    fprintf(fh,"c Vector: (%f,%f,%f)\n",this->c[0],this->c[1],this->c[2]);
    fprintf(fh,"C Vector: (%f,%f,%f)\n",this->C[0],this->C[1],this->C[2]);
    fprintf(fh,"width: %u\n",this->width);
    fprintf(fh,"height: %u\n",this->height);
  }
  else{
    fclose(fh);
    return -1;
  }

  fclose(fh);
  return 0;
}

/*-----------------------------------------------------
  Load PPC parameters from file
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  const char *filepath: file name to load from
Return:
  int: 0 for success, -1 otherwise
-----------------------------------------------------*/
int PPC::load(const char *filepath)
{
  char buf[256];
  float fst, snd, trd;
  int width;
  ifstream ifs (filepath);

  if (ifs){
    ifs.getline(buf,sizeof(buf));
    sscanf(buf, "%*s %*s (%f,%f,%f)",&fst, &snd, &trd);
    Vector3D a = Vector3D(fst,snd,trd);
    ifs.getline(buf,sizeof(buf));
    sscanf(buf, "%*s %*s (%f,%f,%f)",&fst, &snd, &trd);
    Vector3D b = Vector3D(fst,snd,trd);
    ifs.getline(buf,sizeof(buf));
    sscanf(buf, "%*s %*s (%f,%f,%f)",&fst, &snd, &trd);
    Vector3D c = Vector3D(fst,snd,trd);
    ifs.getline(buf,sizeof(buf));
    sscanf(buf, "%*s %*s (%f,%f,%f)",&fst, &snd, &trd);
    Vector3D C = Vector3D(fst,snd,trd);
    ifs.getline(buf,sizeof(buf));
    sscanf(buf, "%*s %d",&width);
    unsigned int w = width;
    ifs.getline(buf,sizeof(buf));
    sscanf(buf, "%*s %d",&width);
    unsigned int h = width;

    this->width = w;
    this->height = h;
    this->C = C;
  	this->a = a;
  	this->b = b;
  	this->c = c;
  }
  else{
    cout << "load file error!" << endl;
    return -1;
  }
  ifs.close();
  return 0;
}

/*-----------------------------------------------------
  base function for Interpolation between 2 cameras
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  PPC *ppc0: starting camera
  PPC *ppc1: ending camera
  float frac: fraction of interpolation
Return:

-----------------------------------------------------*/
void PPC::setInterpolation(PPC *ppc0, PPC *ppc1, float frac)
{
  *this = *ppc0;

	Vector3D C = ppc0->C + (ppc1->C - ppc0->C)*frac;
	Vector3D newvd = (ppc0->getVD() + (ppc1->getVD() - ppc0->getVD())*frac).Normalize();
	Vector3D up = (ppc0->b + (ppc1->b- ppc0->b)*frac)*-1.0f;

	float flen = this->getFocal();
	Vector3D newa = (newvd.CrossProduct(up)).Normalize()*this->a.Length();
	Vector3D newb = (newvd.CrossProduct(newa)).Normalize()*this->b.Length();
	Vector3D newc = (newvd*flen - newb * (float)this->height/2.0f - newa * (float)this->width/2.0f).Normalize()*this->c.Length();

	// commit the new values
	this->C.Set(C);
	this->a.Set(newa);
	this->b.Set(newb);
	this->c.Set(newc);
}

/*-----------------------------------------------------
  reposition camera given C, VD and up vector
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  Vector3D newC: new C vector
  Vector3D newvd: new view direction
  Vector3D up: reference up vector
Return:

-----------------------------------------------------*/
void PPC::rePosition(Vector3D newC, Vector3D newvd, Vector3D up) {

	Vector3D newa, newb, newc;
	newvd = newvd.Normalize();
	float f = this->getFocal();
	newa = (newvd.CrossProduct(up)).Normalize()*this->a.Length();
	newb = (newvd.CrossProduct(newa)).Normalize()*this->b.Length();
	newc = newvd*f - newb * (float)this->height/2.0f - newa * (float)this->width/2.0f;

	// commit the new values
	this->C = newC;
	this->a = newa;
	this->b = newb;
	this->c = newc;

}

/*-----------------------------------------------------
  Recover 3D point given 2D coordinates
-----------------------------------------------------*/
/*-----------------------------------------------------
Arguments:
  float u0: x-coor in framebuffer
  float v0: y-coor in framebuffer
  float zlen: scale length
Return:
  Vector3D: recovered 3D point
-----------------------------------------------------*/
Vector3D PPC::get3DPoint(float u0, float v0, float zlen)
{
	float flen = getFocal();
	Vector3D ret = this->C + (this->c + this->a*u0 + this->b*v0)*zlen / flen;
	return ret;
}

void PPC::setIntrinsicsHW(float zNear, float zFar) {

  unsigned int w = this->getWidth();
  unsigned int h = this->getHeight();
 	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float left, right, bottom, top;
	float scf = zNear / this->getFocal();
	left = -a.Length()*(float)w / 2.0f * scf;
	right = +a.Length()*(float)w / 2.0f * scf;
	bottom = -b.Length()*(float)h / 2.0f*scf;
	top = +b.Length()*(float)h / 2.0f*scf;
	glFrustum(left, right, bottom, top, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);

}

void PPC::setExtrinsicsHW() {

	Vector3D LAP = C + getVD()*100.0f;
	glLoadIdentity();
	//GLKMatrix4MakeLookAt(C[0], C[1], C[2], LAP[0], LAP[1], LAP[2], -b[0], -b[1], -b[2]);
  gluLookAt(C[0], C[1], C[2], LAP[0], LAP[1], LAP[2], -b[0], -b[1], -b[2]);

}
