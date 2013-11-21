#include "Node.h"
#ifdef __gl_h_
  #undef __gl_h_
#endif
#include "Shader.h"


/******************************************************************************
 * The shader group class. Is a group that applies a shader to its children
 ******************************************************************************/
class ShaderGroup : public Group
{
protected:
  Shader* shader;
public:
  ShaderGroup(Shader*);
  ~ShaderGroup();

  void draw(Matrix4, Frustum, bool); //set the shader and draw the child nodes
  void computeBoundingSphere(Matrix4);
};