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

#include "SoGLShaderObject.h"

#include <assert.h>

#include "SoGLARBShaderObject.h"
#include "SoGLCgShaderObject.h"
#include "SoGLSLShaderObject.h"

// *************************************************************************

SoGLShaderObject::SoGLShaderObject(const cc_glglue * g)
{
  this->isActiveFlag = TRUE;
  this->isVertexShaderFlag = TRUE;
  this->glctx = g;
}

const cc_glglue *
SoGLShaderObject::GLContext(void) const
{
  return this->glctx;
}

void
SoGLShaderObject::operator delete(void * obj)
{
  switch (((SoGLShaderObject *)obj)->shaderType()) {
  case SoShaders::ARB_SHADER: ::delete (SoGLARBShaderObject *)obj; break;
  case SoShaders::GLSL_SHADER: ::delete (SoGLSLShaderObject*)obj; break;
  case SoShaders::CG_SHADER: ::delete (SoGLCgShaderObject *)obj; break;
  default: assert(FALSE && "shaderType unknown!");
  }
}

void
SoGLShaderObject::operator delete[](void * obj)
{
  switch (((SoGLShaderObject *)obj)->shaderType()) {
  case SoShaders::ARB_SHADER: ::delete [](SoGLARBShaderObject *)obj; break;
  case SoShaders::GLSL_SHADER: ::delete [](SoGLSLShaderObject*)obj; break;
  case SoShaders::CG_SHADER: ::delete [](SoGLCgShaderObject *)obj; break;
  default: assert(FALSE && "shaderType unknown!");
  }
}

void
SoGLShaderObject::setIsVertexShader(SbBool flag)
{
  if (this->isVertexShaderFlag != flag) {
    this->unload();
    this->isVertexShaderFlag = flag;
  }
}

SbBool
SoGLShaderObject::isVertexShader(void) const
{
  return this->isVertexShaderFlag;
}

void SoGLShaderObject::setIsActive(SbBool flag)
{
  this->isActiveFlag = flag;
}

SbBool
SoGLShaderObject::isActive(void) const
{
  return (!this->isLoaded()) ? FALSE : this->isActiveFlag;
}
