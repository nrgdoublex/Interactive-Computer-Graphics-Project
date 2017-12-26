//#define GEOM_SHADER

#include "CGInterface.h"
#include "Vector3D.h"
#include "FrameBufferWrapper.h"

#include <iostream>

using namespace std;

CGInterface::CGInterface() {};

void CGInterface::PerSessionInit() {

  glEnable(GL_DEPTH_TEST);

  CGprofile latestVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
  CGprofile latestGeometryProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);
  CGprofile latestPixelProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

  if (latestGeometryProfile == CG_PROFILE_UNKNOWN) {
    cerr << "ERROR: geometry profile is not available" << endl;
#ifdef GEOM_SHADER
    exit(0);
#endif
  }

  cgGLSetOptimalOptions(latestGeometryProfile);
  CGerror Error = cgGetError();
  if (Error) {
	  cerr << "CG ERROR: " << cgGetErrorString(Error) << endl;
  }

  cout << "Info: Latest GP Profile Supported: " << cgGetProfileString(latestGeometryProfile) << endl;

  geometryCGprofile = latestGeometryProfile;

  cout << "Info: Latest VP Profile Supported: " << cgGetProfileString(latestVertexProfile) << endl;
  cout << "Info: Latest FP Profile Supported: " << cgGetProfileString(latestPixelProfile) << endl;

  vertexCGprofile = latestVertexProfile;
  pixelCGprofile = latestPixelProfile;
  cgContext = cgCreateContext();


}

bool ShaderOneInterface::PerSessionInit(CGInterface *cgi) {

#ifdef GEOM_SHADER
  geometryProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE,
    "shaderOne.cg", cgi->geometryCGprofile, "GeometryMain", NULL);
  if (geometryProgram == NULL)  {
    CGerror Error = cgGetError();
    cerr << "Shader One Geometry Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
    cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
    return false;
  }
#endif

  vertexProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE,
    "shaderOne.cg", cgi->vertexCGprofile, "VertexMain", NULL);
  if (vertexProgram == NULL) {
    CGerror Error = cgGetError();
    cerr << "Shader One Geometry Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
    cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
    return false;
  }

  fragmentProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE,
    "shaderOne.cg", cgi->pixelCGprofile, "FragmentMain", NULL);
  if (fragmentProgram == NULL)  {
    CGerror Error = cgGetError();
    cerr << "Shader One Fragment Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
    cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
    return false;
  }

	// load programs
#ifdef GEOM_SHADER
	cgGLLoadProgram(geometryProgram);
#endif
	cgGLLoadProgram(vertexProgram);
	cgGLLoadProgram(fragmentProgram);

	// build some parameters by name such that we can set them later...
  vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj" );
  vertexSphereCenter = cgGetNamedParameter(vertexProgram, "sphereCenter");
  vertexSphereRadius = cgGetNamedParameter(vertexProgram, "sphereRadius");
  vertexAnimationFraction = cgGetNamedParameter(vertexProgram, "animationFraction");
  geometryModelViewProj = cgGetNamedParameter(geometryProgram, "modelViewProj" );
  geometryAnimationFraction = cgGetNamedParameter(geometryProgram, "animationFraction");

  fragmentModelViewProj = cgGetNamedParameter(fragmentProgram, "modelViewProj" );
  fragmentEye = cgGetNamedParameter(fragmentProgram, "eye" );
  fragmentTLC = cgGetNamedParameter(fragmentProgram, "tlc" );
  fragmentTRC = cgGetNamedParameter(fragmentProgram, "trc" );
  fragmentBLC = cgGetNamedParameter(fragmentProgram, "blc" );
  fragmentWidth = cgGetNamedParameter(fragmentProgram, "w" );
  fragmentHeight = cgGetNamedParameter(fragmentProgram, "h" );
  fragmentPixel = cgGetNamedParameter(fragmentProgram, "pixel" );
  fragmentCubeMap = cgGetNamedParameter(fragmentProgram, "cubemap" );

  return true;

}

void ShaderOneInterface::PerFrameInit() {

	//set parameters
	cgGLSetStateMatrixParameter(
    vertexModelViewProj,
		CG_GL_MODELVIEW_PROJECTION_MATRIX,
    CG_GL_MATRIX_IDENTITY);

  cgGLSetStateMatrixParameter(
    geometryModelViewProj,
		CG_GL_MODELVIEW_PROJECTION_MATRIX,
    CG_GL_MATRIX_IDENTITY);

  cgGLSetStateMatrixParameter(
    fragmentModelViewProj,
		CG_GL_MODELVIEW_PROJECTION_MATRIX,
    CG_GL_MATRIX_IDENTITY);
  Vector3D eye =  wrapper->ppc->getC();
  cgGLSetParameter3fv(fragmentEye,(float *)&eye);
  Vector3D tlc = wrapper->bbppc->getC() + wrapper->bbppc->getc();
  cgGLSetParameter3fv(fragmentTLC,(float *)&tlc);
  Vector3D trc = tlc + wrapper->bbppc->geta()*wrapper->bbppc->getWidth();
  cgGLSetParameter3fv(fragmentTRC,(float *)&trc);
  Vector3D blc = tlc + wrapper->bbppc->getb()*wrapper->bbppc->getHeight();
  cgGLSetParameter3fv(fragmentBLC,(float *)&blc);
  float width = (float) wrapper->bbppc->getWidth();
  cgGLSetParameter1f(fragmentWidth,width);
  float height = (float) wrapper->bbppc->getHeight();
  cgGLSetParameter1f(fragmentHeight,height);
  cgGLSetTextureParameter(fragmentPixel,wrapper->tid);
  cgGLEnableTextureParameter(fragmentPixel);
  cgGLSetTextureParameter(fragmentCubeMap,wrapper->cmid);
  cgGLEnableTextureParameter(fragmentCubeMap);
}

void ShaderOneInterface::PerFrameDisable() {
}


void ShaderOneInterface::BindPrograms() {

#ifdef GEOM_SHADER
  cgGLBindProgram(geometryProgram);
#endif
  cgGLBindProgram(vertexProgram);
  cgGLBindProgram(fragmentProgram);

}

void CGInterface::DisableProfiles() {

  cgGLDisableProfile(vertexCGprofile);
#ifdef GEOM_SHADER
  cgGLDisableProfile(geometryCGprofile);
#endif
  cgGLDisableProfile(pixelCGprofile);

}

void CGInterface::EnableProfiles() {

  cgGLEnableProfile(vertexCGprofile);
#ifdef GEOM_SHADER
  cgGLEnableProfile(geometryCGprofile);
#endif
  cgGLEnableProfile(pixelCGprofile);

}
