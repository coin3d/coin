/*** -*- Mode: C++ -*- *****************************************************
 *
 *
 ***************************************************************************/

#ifndef __SO_GL_SHADER_OBJECT_H__
#define __SO_GL_SHADER_OBJECT_H__

#include <Inventor/nodes/SoGLShaderTypes.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>

class SoGLShaderParameter;

class SoGLShaderObject
{
public: // SoGLShaderObject protocol interface:

  virtual SbBool isLoaded() const                                       = 0;
  virtual void load(const char* sourceString)                           = 0;
  virtual void unload()                                                 = 0;
  virtual SoGLShader::ShaderType shaderType() const                     = 0;
  virtual SoGLShaderParameter*  
  getParameter(int index, const char* name, SoGLShader::ValueType type) = 0;

public: // 
  // SoGLShaderObject implements its own delete operator
  void operator delete(void* obj);
  void operator delete[](void* obj);

  void setIsVertexShader(SbBool flag);
  SbBool isVertexShader() const;

  void  setIsActive(SbBool flag);
  SbBool isActive() const;

public:
  SoGLShaderObject();

#if defined(SOURCE_HINT)
  SbString sourceHint; // either the file name or the first line of source code
#endif

private:
  SbBool isVertexShaderFlag;
  SbBool isActiveFlag;
};

#endif /* __SO_GL_SHADER_OBJECT_H__ */
