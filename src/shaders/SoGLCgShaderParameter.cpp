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

#include "SoGLCgShaderParameter.h"

#include <assert.h>

#include <Inventor/errors/SoDebugError.h>
#include "SoGLCgShaderObject.h"

// *************************************************************************

SoGLCgShaderParameter::SoGLCgShaderParameter(SoGLCgShaderObject* shader,
                                             const char* name,
                                             SoShader::ValueType type)
  : SoGLShaderParameter()
{
  this->cgParameter = glue_cgGetNamedParameter(shader->cgProgram, name);
  this->type = SoShader::UNKNOWN_TYPE;
  this->cgProgram = &shader->cgProgram;

  this->ensureParameter(name, type);
}

SoGLCgShaderParameter::~SoGLCgShaderParameter()
{
}

SoShader::ShaderType
SoGLCgShaderParameter::shaderType() const
{
  return SoShader::CG_SHADER;
}

SbBool SoGLCgShaderParameter::isReferenced()
{ 
  return (glue_cgIsParameter(this->cgParameter));
}

void
SoGLCgShaderParameter::setState(CGGLenum matrix, CGGLenum transform,
                                const char* name)
{
  if (this->ensureParameter(name, SoShader::FLOAT_MATRIX4))
    glue_cgGLSetStateMatrixParameter(this->cgParameter, matrix, transform);
}

void
SoGLCgShaderParameter::set1f(const cc_glglue * g, const float v,
                             const char* name, const int)
{
  if (this->ensureParameter(name, SoShader::FLOAT))
    glue_cgGLSetParameter1f(this->cgParameter, v);
}

void
SoGLCgShaderParameter::set2f(const cc_glglue * g, const float * v,
                             const char* name, const int)
{
  if (this->ensureParameter(name, SoShader::FLOAT2))
    glue_cgGLSetParameter2f(this->cgParameter, v[0], v[1]);
}

void
SoGLCgShaderParameter::set3f(const cc_glglue * g, const float * v,
                             const char* name, const int)
{
  if (this->ensureParameter(name, SoShader::FLOAT3))
    glue_cgGLSetParameter3f(this->cgParameter, v[0], v[1], v[2]);
}

void
SoGLCgShaderParameter::set4f(const cc_glglue * g, const float * v,
                             const char* name, const int)
{
  if (this->ensureParameter(name, SoShader::FLOAT4))
    glue_cgGLSetParameter4f(this->cgParameter, v[0], v[1], v[2],v[3]); 
}

SbBool
SoGLCgShaderParameter::isCorrectType(SoShader::ValueType theType) 
{
  return (this->type == theType || theType == SoShader::UNKNOWN_TYPE);
}

SbBool
SoGLCgShaderParameter::ensureParameter(const char* name, SoShader::ValueType theType)
{
  if (this->isCorrectType(theType) && this->isReferenced()) {
    // std::cerr << "cgParam is cached!" << std::endl;
    return TRUE;
  }

  this->cgParameter = glue_cgGetNamedParameter(*this->cgProgram, name);
  this->type = SoShader::UNKNOWN_TYPE;

  if (!this->isReferenced()) return FALSE;

  CGtype cgType = glue_cgGetParameterType(this->cgParameter);

  this->type = SoGLCgShaderParameter::getParameterTypeFor(cgType);

  if (!this->isCorrectType(theType)) {
    SoDebugError::postWarning("SoGLCgShaderParameter::ensureParameter",
                              "In main(): parameter %s [%s] is of wrong type!",
                              name, glue_cgGetTypeString(cgType));
    return FALSE;
  }
  else
    return this->isReferenced();
}

// --- static methods -------------------------------------------------------

SoShader::ValueType
SoGLCgShaderParameter::getParameterTypeFor(CGtype type)
{
  switch (type) {
  case CG_FLOAT:
  case CG_FLOAT1: return SoShader::FLOAT;
  case CG_FLOAT2: return SoShader::FLOAT2;
  case CG_FLOAT3: return SoShader::FLOAT3;
  case CG_FLOAT4: return SoShader::FLOAT4;
  case CG_SAMPLER1D: return SoShader::TEXTURE1D;
  case CG_SAMPLER2D: return SoShader::TEXTURE2D;
  case CG_SAMPLER3D: return SoShader::TEXTURE3D;
  case CG_SAMPLERCUBE: return SoShader::TEXTURE_CUBE;
  case CG_SAMPLERRECT: return SoShader::TEXTURE_RECT;
  case CG_FLOAT2x2: return SoShader::FLOAT_MATRIX2;
  case CG_FLOAT3x3: return SoShader::FLOAT_MATRIX3;
  case CG_FLOAT4x4: return SoShader::FLOAT_MATRIX4;
  case CG_UNKNOWN_TYPE: return SoShader::UNKNOWN_TYPE;
  default: 
    SoDebugError::post("SoGLCgShaderParameter::getParameterTypeFor",
                       "cannot map CGtype to ParameterType");
    return SoShader::UNKNOWN_TYPE;
  }
}
