#ifndef COIN_SOGLARBSHADER_H
#define COIN_SOGLARBSHADER_H

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


#endif /* ! COIN_SOGLARBSHADER_H */
