#ifndef _FBO_H_
#define _FBO_H_

#include <iostream>
#include <stdlib.h>
#include "GLee.h"
#include <GL/glut.h>

class FBO
{
protected:
  GLuint fboId, fboTex, fboMask, rbo_depth_stencil;

  unsigned int width, height;

  bool isValidFBO;

  bool isActivated;
  bool depthStencilBuf;
  bool maskBuf;

public:
  FBO(unsigned int w, unsigned int h);
  ~FBO();

  void activate();
  void deactivate();

  void activateTexture();
  void deactivateTexture();

  void activateMask();
  void deactivateMask();

  void generateColorOnly();
  void generateColorAndMask();
  void generateColorAndDepth();
  void generate();
  void destroy();

  void updateSize(unsigned int w, unsigned int h);
};


#endif