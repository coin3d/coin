/*** -*- Mode: C++ -*- *****************************************************
 *
 *
 ***************************************************************************/

#ifndef _SO_GL_SHADER_PARAMETER_H__
#define _SO_GL_SHADER_PARAMETER_H__

#include <Inventor/SbBasic.h>
#include "SoGLShaderTypes.h"

class SoGLShaderParameter
{
public:
  virtual SbBool isReferenced()                       = 0;

  virtual void set1f(const float value)               = 0;
  virtual void set2f(const float *value)              = 0;
  virtual void set3f(const float *value)              = 0;
  virtual void set4f(const float *value)              = 0;

  virtual SoGLShader::ShaderType shaderType() const   = 0;

public:
  virtual SbBool isFloat();
  virtual SbBool isFloat2();
  virtual SbBool isFloat3();
  virtual SbBool isFloat4();
  virtual SbBool isTexture();

public: 
  SoGLShaderParameter();

  // the SoGLShaderParameter implements its own delete operator 
  void operator delete(void *obj);
  void operator delete[](void *obj);

protected:
  SoGLShader::ValueType type;
};

#endif /* _SO_GL_SHADER_PARAMETER_H__ */
