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

#include "SoGLSLShaderParameter.h"
#include "SoGLSLShaderObject.h"

#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

SoGLSLShaderParameter::SoGLSLShaderParameter(void)
{
  this->location  = -1;
  this->cacheType = GL_FLOAT;
  this->cacheName = "";
  this->cacheSize =  0;
}

SoGLSLShaderParameter::~SoGLSLShaderParameter()
{
}

SoShader::Type
SoGLSLShaderParameter::shaderType(void) const
{ 
  return SoShader::GLSL_SHADER;
}

void
SoGLSLShaderParameter::set1f(const SoGLShaderObject * shader,
			     const float value, const char *name, const int)
{
  if (this->isValid(shader, name, GL_FLOAT))
    shader->GLContext()->glUniform1fARB(this->location, value);
}

void
SoGLSLShaderParameter::set2f(const SoGLShaderObject * shader,
			     const float * value, const char *name, const int)
{
  if (this->isValid(shader, name, GL_FLOAT_VEC2_ARB))
    shader->GLContext()->glUniform2fARB(this->location, value[0], value[1]);
}

void
SoGLSLShaderParameter::set3f(const SoGLShaderObject * shader,
			     const float * v, const char *name, const int)
{
  if (this->isValid(shader, name, GL_FLOAT_VEC3_ARB))
    shader->GLContext()->glUniform3fARB(this->location, v[0], v[1], v[2]);
}

void
SoGLSLShaderParameter::set4f(const SoGLShaderObject * shader,
			     const float * v, const char *name, const int)
{
  if (this->isValid(shader, name, GL_FLOAT_VEC4_ARB))
    shader->GLContext()->glUniform4fARB(this->location, v[0], v[1], v[2], v[3]);
}


void
SoGLSLShaderParameter::set1fv(const SoGLShaderObject * shader, const int num,
			      const float *value, const char * name, const int)
{
  int cnt = num;
  if (this->isValid(shader, name, GL_FLOAT, &cnt))
    shader->GLContext()->glUniform1fvARB(this->location, cnt, value);
}

void
SoGLSLShaderParameter::set2fv(const SoGLShaderObject * shader, const int num,
			      const float* value, const char* name, const int)
{
  int cnt = num;
  if (this->isValid(shader, name, GL_FLOAT_VEC2_ARB, &cnt))
    shader->GLContext()->glUniform2fvARB(this->location, cnt, value);
}

void
SoGLSLShaderParameter::set3fv(const SoGLShaderObject * shader, const int num,
			      const float* value, const char * name, const int)
{
  int cnt = num;
  if (this->isValid(shader, name, GL_FLOAT_VEC3_ARB, &cnt))
    shader->GLContext()->glUniform3fvARB(this->location, cnt, value);
}

void
SoGLSLShaderParameter::set4fv(const SoGLShaderObject * shader, const int num,
			      const float* value, const char * name, const int)
{
  int cnt = num;
  if (this->isValid(shader, name, GL_FLOAT_VEC4_ARB, &cnt))
    shader->GLContext()->glUniform4fvARB(this->location, cnt, value);
}

void
SoGLSLShaderParameter::setMatrix(const SoGLShaderObject *,
				 const float *, const char *,
				 const int)
{
  // FIXME not implemented yet -- 20050128 martin
}

  
void
SoGLSLShaderParameter::setMatrixArray(const SoGLShaderObject *,
				      const int, const float *,
				      const char *, const int)
{
  // FIXME not implemented yet -- 20050128 martin
}


SbBool
SoGLSLShaderParameter::isValid(const SoGLShaderObject * shader,
			       const char * name, GLenum type,
			       int * num)
{
  assert(shader);
  assert(shader->shaderType() == SoShader::GLSL_SHADER);
  
  if (this->location>-1 && this->cacheName==name && this->cacheType==type) {
    if (num) { // assume: ARRAY
      if (this->cacheSize < *num) {
	// FIXME: better error handling - 20050128 martin
	SoDebugError::postWarning("SoGLSLShaderParameter::isValid",
				  "parameter %s[%d] < input[%d]!",
				  this->cacheName.getString(),
				  this->cacheSize, *num);
	*num = this->cacheSize;
      }
      return (*num > 0);
    }
    return TRUE;
  }
 
  COIN_GLhandle pHandle = ((SoGLSLShaderObject*)shader)->programHandle;
  const cc_glglue * g = shader->GLContext();

  this->cacheSize = 0;  
  this->location  = g->glGetUniformLocationARB(pHandle,
					       (const COIN_GLchar *)name);

  if (this->location == -1)  return FALSE;

  GLsizei     length;
  COIN_GLchar myName[128]; // FIXME: check this 20050128 martin
    
  g->glGetActiveUniformARB(pHandle, this->location, 128, &length,
			   &this->cacheSize, &this->cacheType, myName);

  this->cacheName = name;

  if (this->location == -1) return FALSE;
  if (this->cacheType != type) return FALSE;
  
  if (num) { // assume: ARRAY
    if (this->cacheSize < *num) {
      // FIXME: better error handling - 20050128 martin
      SoDebugError::postWarning("SoGLSLShaderParameter::isValid",
				"parameter %s[%d] < input[%d]!",
				this->cacheName.getString(),
				this->cacheSize, *num);
      *num = this->cacheSize;
    }
    return (*num > 0);
  }
  return TRUE;
}
