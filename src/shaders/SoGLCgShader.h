/* *** -*- Mode: C++ -*- ****************************************************
 *
 * **************************************************************************/

#ifndef __SO_GL_CG_SHADER_H__
#define __SO_GL_CG_SHADER_H__

#if defined(SO_CG_SHADER_SUPPORT)

#include "SoGLShaderParameter.h"
#include "SoGLShaderObject.h"

#include <Cg/cgGL.h>

class SoGLCgShaderObject;

// --- SoGLCgShaderParameter ------------------------------------------------

class SoGLCgShaderParameter : public SoGLShaderParameter
{
 public:
  virtual inline SbBool isReferenced() { return this->isReferencedFlag; }

  virtual void set1f(const float value);
  virtual void set2f(const float *value);
  virtual void set3f(const float *value);
  virtual void set4f(const float *value);

  virtual SoGLShader::ShaderType shaderType() const;

 public:
  SoGLCgShaderParameter(SoGLCgShaderObject*   shader, 
			const char*           name,
			SoGLShader::ValueType type);

  void setState(CGGLenum matrix, CGGLenum transform);

 private:
  CGparameter cgParameter;
  SbBool      isReferencedFlag;

  static SoGLShader::ValueType getParameterTypeFor(CGtype type);
};


// --- SoGLCgShaderObject ----------------------------------------------------

class SoGLCgShaderObject : public SoGLShaderObject
{
  friend class SoGLCgShaderParameter;
  friend class SoGLCgShaderProgram;
  
public: // SoGLShaderObject protocol interface
  virtual SbBool isLoaded() const;
  virtual void load(const char * sourceString);
  virtual void unload();
  virtual SoGLShader::ShaderType shaderType() const;
  virtual SoGLShaderParameter* 
  getParameter(int index, const char* name, SoGLShader::ValueType type);

public:
  SoGLCgShaderObject();
  virtual ~SoGLCgShaderObject();
 
private:
  CGprofile getProfile() const;

  virtual void enable();
  virtual void disable();
  
  CGprogram cgProgram;
  CGprofile cgProfile;

private:
  static CGcontext cgContext;
  static int  instanceCount;
  static void ensureCgContext();
  static void destroyCgContext();
  static void cgErrorCallback();
  static void printError(CGerror error, CGcontext context);
};


// --- SoGLCgShaderProgram -------------------------------------------------

class SoGLCgShaderProgram
{
public:
  SoGLCgShaderProgram();
  ~SoGLCgShaderProgram();

  void addShaderObject(SoGLCgShaderObject * shaderObject);
  void removeShaderObject(SoGLCgShaderObject * shaderObject);

  void enable();
  void disable();

#if defined(SOURCE_HINT)
  SbString getSourceHint() const;
#endif

protected:
  SoGLCgShaderObject * fragmentShader;
  SoGLCgShaderObject * vertexShader;
};
#endif /* SO_CG_SHADER_SUPPORT */

#endif /* __SO_GL_CG_SHADER_H__ */
