#ifndef COIN_SOGLSLSHADER_H
#define COIN_SOGLSLSHADER_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

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

  virtual void set1f(const float  value, const char* name, const int id);
  virtual void set2f(const float *value, const char* name, const int id);
  virtual void set3f(const float *value, const char* name, const int id);
  virtual void set4f(const float *value, const char* name, const int id);

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

#endif /* ! COIN_SOGLSLSHADER_H */
