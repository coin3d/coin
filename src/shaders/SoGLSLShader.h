/**** -*- Mode: C++ -*- ************************************************************
 ***                                                                     ***
 ***************************************************************************/




#ifndef __SO_GL_SL_SHADER_H__
#define __SO_GL_SL_SHADER_H__

#if defined(SO_GLSL_SHADER_SUPPORT)

#include "SoGLShaderParameter.h"
#include "SoGLShaderObject.h"

#include <vector>
#include <GL/gl.h>
#include <GL/glext.h>

class SoGLSLShaderParameter : public SoGLShaderParameter
{
 public: // Parameter protocol interface
  virtual inline SbBool isTexture()    { return FALSE;                 }
  virtual inline SbBool isReferenced() { return (this->location >= 0); }

  virtual void set1f(const float value);
  virtual void set2f(const float *value);
  virtual void set3f(const float *value);
  virtual void set4f(const float *value);

  virtual SoGLShader::ShaderType shaderType() const;

 public:
  SoGLSLShaderParameter();
  SoGLSLShaderParameter(GLhandleARB           program, 
			const char*           name, 
			SoGLShader::ValueType type);
  virtual ~SoGLSLShaderParameter();


 private:
  GLint    location;
  SbString name;

  static SoGLShader::ValueType getParameterTypeFor(GLenum type);
};

class SoGLSLShaderObject : public SoGLShaderObject
{

public: // SoGLShaderObject protocol interface
  virtual SoGLShader::ShaderType shaderType() const;

  virtual SoGLShaderParameter* 
  getParameter(int index, const char* name, SoGLShader::ValueType type);

  virtual SbBool isLoaded() const;
  virtual void load(const char* sourceString);
  virtual void unload();

public:
  void attach(GLhandleARB programHandle);
  void detach();

public:
  SoGLSLShaderObject();
  virtual ~SoGLSLShaderObject();

public: // static methods
  // objType 0: program 1: vertexShader 2: fragmentShader
  static SbBool didOpenGLErrorOccur(int objType);
  static void printInfoLog(GLhandleARB handle, int objType);
 
protected:
  GLhandleARB programHandle;
  GLhandleARB shaderHandle;
};

class SoGLSLShaderProgram
{
public:
  void addShaderObject(SoGLSLShaderObject *shaderObject);
  void removeShaderObject(SoGLSLShaderObject *shaderObject);
  void enable();
  void disable();
  void postShouldLink();

#if defined(SOURCE_HINT)
  SbString getSourceHint() const;
#endif
  
public:
  SoGLSLShaderProgram();
  ~SoGLSLShaderProgram();

protected:
  std::vector<SoGLSLShaderObject*> shaderObjects;
  GLhandleARB                      programHandle;
  SbBool                           shouldLink;
  SbBool                           isExecutable;

  int  indexOfShaderObject(SoGLSLShaderObject *shaderObject);
  void ensureLinking();
  void ensureProgramHandle();
};

#endif /* SO_GLSL_SHADER_SUPPORT */

#endif /*__SO_GL_SL_SHADER_H__ */

