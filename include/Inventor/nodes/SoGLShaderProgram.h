/*** -*- Mode: C++ -*- *****************************************************
 *
 *
 ***************************************************************************/

#ifndef __SO_GL_SHADER_PROGRAM_H__
#define __SO_GL_SHADER_PROGRAM_H__

#include <Inventor/SbString.h>

class SoGLShaderObject;

class SoGLShaderProgram
{
public:
  SoGLShaderProgram();
  ~SoGLShaderProgram();
  void addShaderObject(SoGLShaderObject * shaderObject);
  void removeShaderObject(SoGLShaderObject * shaderObject);
  void enable();
  void disable();
  void postShouldLink();

#if defined(SOURCE_HINT)
  SbString getSourceHint();
#endif
  
private:
  class SoGLARBShaderProgram * arbShaderProgram;
  class SoGLCgShaderProgram  * cgShaderProgram;
  class SoGLSLShaderProgram  * glslShaderProgram;
};

#endif /* __SO_GL_SHADER_PROGRAM_H__ */
