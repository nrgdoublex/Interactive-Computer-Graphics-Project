#ifndef CGINTERFACE_H
#define CGINTERFACE_H

#include "Cg/cgGL.h"
#include "Cg/cg.h"


// two classes defining the interface between the CPU and GPU

// models part of the CPU-GPU interface that is independent of specific shaders
class CGInterface {

public:
  CGprofile   vertexCGprofile; // vertex shader profile of GPU
	CGprofile   geometryCGprofile; // geometry shader profile of GPU
	CGprofile   pixelCGprofile; // pixel shader profile of GPU
	CGcontext  cgContext;
  void PerSessionInit(); // per session initialization
  CGInterface(); // constructor
  void EnableProfiles(); // enable GPU rendering
  void DisableProfiles(); // disable GPU rendering
};


// models the part of the CPU-GPU interface for a specific shader
//        here there is a single shader "ShaderOne"
//  a shader consists of a vertex, a geometry, and a pixel (fragment) shader
// fragment == pixel; shader == program; e.g. pixel shader, pixel program, fragment shader, fragment program, vertex shader, etc.
class ShaderOneInterface {

  CGprogram geometryProgram; // the geometry shader to be used for the "ShaderOne"
  CGprogram vertexProgram;
  CGprogram fragmentProgram;
  // uniform parameters, i.e parameters that have the same value for all geometry rendered
  CGparameter vertexModelViewProj; // a matrix combining projection and modelview matrices
  CGparameter vertexSphereRadius, vertexSphereCenter, vertexAnimationFraction;
  CGparameter geometryModelViewProj; // geometry shader
  CGparameter geometryAnimationFraction;

  CGparameter fragmentEye;
  CGparameter fragmentTLC;
  CGparameter fragmentTRC;
  CGparameter fragmentBLC;
  CGparameter fragmentWidth;
  CGparameter fragmentHeight;
  CGparameter fragmentPixel;
  CGparameter fragmentCubeMap;
  CGparameter fragmentModelViewProj;

public:
  ShaderOneInterface() {};
  bool PerSessionInit(CGInterface *cgi); // per session initialization
  void BindPrograms(); // enable geometryProgram, vertexProgram, fragmentProgram
  void PerFrameInit(); // set uniform parameter values, etc.
  void PerFrameDisable(); // disable programs

};

#endif
