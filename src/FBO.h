#ifndef _FBO_H_
#define _FBO_H_

class FBO
{
protected:
  GLuint fboId, fboTex, rboDepth;

  unsigned int width, height;

  bool isActivated;
  bool depthBuf;

public:
  FBO(unsigned int w, unsigned int h);
  ~FBO();

  void activate();
  void deactivate();

  void generate();
  void destroy();

  void updateSize(unsigned int w, unsigned int h);
};


#endif