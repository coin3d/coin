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

#include "SoGLARBShaderParameter.h"
#include "SoGLARBShaderObject.h"

#include <string.h>

#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

SoGLARBShaderParameter::SoGLARBShaderParameter(void)
{
  this->target = 0;
  this->identifier = 0;
}

SoGLARBShaderParameter::~SoGLARBShaderParameter()
{
}

SoShader::Type
SoGLARBShaderParameter::shaderType(void) const
{
  return SoShader::ARB_SHADER;
}

void
SoGLARBShaderParameter::set1f(const SoGLShaderObject * shader,
                              const float value, const char*, const int idx)
{
  if (this->isValid(shader, idx))
    cc_glglue_glProgramLocalParameter4f(shader->GLContext(), 
					this->target, this->identifier,
					value, value, value, value);
}

void
SoGLARBShaderParameter::set2f(const SoGLShaderObject * shader,
                              const float * value, const char*, const int idx)
{
  if (this->isValid(shader, idx))
    cc_glglue_glProgramLocalParameter4f(shader->GLContext(), 
					this->target, this->identifier,
					value[0], value[1], value[0], value[0]);
}

void
SoGLARBShaderParameter::set3f(const SoGLShaderObject * shader,
                              const float * value, const char*, const int idx)
{
  if (this->isValid(shader, idx))
    cc_glglue_glProgramLocalParameter4f(shader->GLContext(), 
					this->target, this->identifier,
					value[0], value[1], value[2], value[0]);
}

void
SoGLARBShaderParameter::set4f(const SoGLShaderObject * shader,
                              const float * value, const char*, const int idx)
{
  if (this->isValid(shader, idx))
    cc_glglue_glProgramLocalParameter4f(shader->GLContext(), 
					this->target, this->identifier,
					value[0], value[1], value[2], value[3]);
}

void
SoGLARBShaderParameter::set1fv(const SoGLShaderObject*, const int,
			       const float*, const char*, const int)
{
  // FIXME: not implemented yet 20050127 martin
}

void
SoGLARBShaderParameter::set2fv(const SoGLShaderObject*, const int,
			       const float*, const char*, const int)
{
  // FIXME: not implemented yet 20050127 martin
}

void
SoGLARBShaderParameter::set3fv(const SoGLShaderObject*, const int,
			       const float*, const char*, const int)
{
  // FIXME: not implemented yet 20050127 martin
}

void
SoGLARBShaderParameter::set4fv(const SoGLShaderObject*, const int,
			       const float*, const char*, const int)
{
  // FIXME: not implemented yet 20050127 martin
}

void
SoGLARBShaderParameter::setMatrix(const SoGLShaderObject *,
				  const float *, const char *, const int)
{
  // FIXME not implemented yet -- 20050128 martin
}

  
void
SoGLARBShaderParameter::setMatrixArray(const SoGLShaderObject *, const int,
				       const float *, const char *, const int)
{
  // FIXME not implemented yet -- 20050128 martin
}


//FIXME: no type checking implemented 20050128 martin
SbBool 
SoGLARBShaderParameter::isValid(const SoGLShaderObject * shader, const int idx)
{
  assert(shader);
  assert(shader->shaderType() == SoShader::ARB_SHADER);

  this->target     = ((SoGLARBShaderObject*)shader)->target;
  this->identifier = idx;
  return TRUE;
}
