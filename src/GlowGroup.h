#include "Node.h"
#ifdef __gl_h_
  #undef __gl_h_
#endif
#include "Shader.h"


/******************************************************************************
 * The Glow group class. Makes all of its children Glow... or not Glow
 ******************************************************************************/
class GlowGroup : public Group
{
protected:
  float glow;
  Shader* glowShader;

public:
  GlowGroup(Shader*, bool);
  ~GlowGroup();

  void draw(Matrix4, Frustum, bool); //set the shader and draw the child nodes
  void computeBoundingSphere(Matrix4);
};