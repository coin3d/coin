#ifndef COIN_SOGLCGSHADER_H
#define COIN_SOGLCGSHADER_H

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

#if defined(SO_CG_SHADER_SUPPORT)

#include "SoGLShaderParameter.h"
#include "SoGLShaderObject.h"

#include <Cg/cgGL.h>

class SoGLCgShaderObject;

// --- SoGLCgShaderParameter ------------------------------------------------

class SoGLCgShaderParameter : public SoGLShaderParameter
{
 public:
  virtual inline SbBool isReferenced();

  virtual void set1f(const float value,  const char* name, const int id);
  virtual void set2f(const float *value, const char* name, const int id);
  virtual void set3f(const float *value, const char* name, const int id);
  virtual void set4f(const float *value, const char* name, const int id);

  virtual SoGLShader::ShaderType shaderType() const;

 public:
  void setState(CGGLenum matrix, CGGLenum transform, const char* name);

 public:
  SoGLCgShaderParameter(SoGLCgShaderObject*   shader, 
			const char*           name,
			SoGLShader::ValueType type);
  virtual ~SoGLCgShaderParameter();

 private:
  CGprogram*  cgProgram;
  CGparameter cgParameter;

  SbBool isCorrectType(SoGLShader::ValueType theType);
  static SoGLShader::ValueType getParameterTypeFor(CGtype type);
  SbBool ensureParameter(const char* name, SoGLShader::ValueType theType);
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

#endif /* ! COIN_SOGLCGSHADER_H */
