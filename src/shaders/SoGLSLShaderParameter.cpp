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

#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

SoGLSLShaderParameter::SoGLSLShaderParameter(const cc_glglue * g,
                                             COIN_GLhandle program,
                                             const char * theName)
{
  this->location =
    g->glGetUniformLocationARB(program, (const COIN_GLchar *)theName);
  if (this->location == -1)
    this->type = SoShader::UNKNOWN_TYPE;
  else {
    GLsizei length, size;
    GLenum type;
    COIN_GLchar name[128];

    g->glGetActiveUniformARB(program,this->location,128,&length,&size,&type,name);
    this->type = SoGLSLShaderParameter::getParameterTypeFor(type);
    if (this->type == SoShader::UNKNOWN_TYPE) 
      this->location = -1;
    else
      this->name = theName;
  }
}

SoGLSLShaderParameter::~SoGLSLShaderParameter()
{
}

// *************************************************************************

SoShader::ShaderType
SoGLSLShaderParameter::shaderType(void) const
{ 
  return SoShader::GLSL_SHADER;
}

// *************************************************************************

void
SoGLSLShaderParameter::set1f(const cc_glglue * g, const float value,
                             const char *, const int)
{
  if (this->isReferenced() && isFloat()) {
    g->glUniform1fARB(this->location, value);
  }
}

void
SoGLSLShaderParameter::set2f(const cc_glglue * g, const float * value,
                             const char *, const int)
{
  if (this->isReferenced() && isFloat2()) {
    g->glUniform2fARB(this->location, value[0], value[1]);
  }
}

void
SoGLSLShaderParameter::set3f(const cc_glglue * g, const float * value,
                             const char *, const int)
{
  if (this->isReferenced() && isFloat3()) {
    g->glUniform3fARB(this->location, value[0], value[1], value[2]);
  }
}

void
SoGLSLShaderParameter::set4f(const cc_glglue * g, const float * value,
                             const char *, const int)
{
  if (this->isReferenced() && isFloat4()) {
    g->glUniform4fARB(this->location, value[0], value[1],value[2],value[3]);
  }
}

// *************************************************************************

SoShader::ValueType
SoGLSLShaderParameter::getParameterTypeFor(GLenum type)
{
  switch (type) {
  case GL_FLOAT: return SoShader::FLOAT;
  case GL_FLOAT_VEC2_ARB: return SoShader::FLOAT2;
  case GL_FLOAT_VEC3_ARB: return SoShader::FLOAT3;
  case GL_FLOAT_VEC4_ARB: return SoShader::FLOAT4;
  case GL_FLOAT_MAT2_ARB: return SoShader::FLOAT_MATRIX2;
  case GL_FLOAT_MAT3_ARB: return SoShader::FLOAT_MATRIX3;
  case GL_FLOAT_MAT4_ARB: return SoShader::FLOAT_MATRIX4;
  default:
    SoDebugError::post("SoGLSLShaderParameter::getParameterTypeFor",
                       "Cannot map GLtype to ValueType");
    return SoShader::UNKNOWN_TYPE;
  }
}
