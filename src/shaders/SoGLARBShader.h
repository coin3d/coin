/*** -*- Mode: C++ -*- *****************************************************
 *
 *
 ***************************************************************************/

#ifndef __SO_GL_ARB_SHADER_H__
#define __SO_GL_ARB_SHADER_H__

#if defined(SO_ARB_SHADER_SUPPORT)

#include "SoGLShaderParameter.h"
#include "SoGLShaderObject.h"
#include <GL/gl.h>

// --- SoGLARBShaderParameter -----------------------------------------------

class SoGLARBShaderParameter : public SoGLShaderParameter
{
 public: // satisfy SoGLShaderParameter protocol interface
  virtual inline SbBool isReferenced() { return FALSE; }

  virtual SoGLShader::ShaderType shaderType() const;

  virtual void set1f(const float  value, const char* name, const int id);
  virtual void set2f(const float *value, const char* name, const int id);
  virtual void set3f(const float *value, const char* name, const int id);
  virtual void set4f(const float *value, const char* name, const int id);

public:
  SoGLARBShaderParameter(GLenum target, GLuint index);
  virtual ~SoGLARBShaderParameter();

private:
  GLenum target;
  GLuint identifier;
};

// --- SoGLARBShaderObject --------------------------------------------------

class SoGLARBShaderObject : public SoGLShaderObject
{
public:
  virtual SbBool isLoaded() const;
  virtual void load(const char *sourceString);
  virtual void unload();
  virtual SoGLShader::ShaderType shaderType() const;
  virtual SoGLShaderParameter* 
  getParameter(int index, const char* name, SoGLShader::ValueType type);

public:
  SoGLARBShaderObject();
  virtual ~SoGLARBShaderObject();
  void enable();
  void disable();
  
private:
  GLuint arbProgramID;
  GLenum target; // either GL_FRAGMENT_PROGRAM_ARB or GL_VERTEX_PROGRAM_ARB
};


// --- SoGLARBShaderProgram -------------------------------------------------

class SoGLARBShaderProgram
{
public:
  SoGLARBShaderProgram();
  
  virtual void addShaderObject(SoGLARBShaderObject *shaderObject);
  virtual void removeShaderObject(SoGLARBShaderObject *shaderObject);

  virtual void enable();
  virtual void disable();

#if defined(SOURCE_HINT)
  virtual SbString getSourceHint() const;
#endif

protected:
  SoGLARBShaderObject * fragmentShader;
  SoGLARBShaderObject * vertexShader;
};
#endif /* SO_ARB_SHADER_SUPPORT */


#endif /* __SO_GL_ARB_SHADER_H__ */
