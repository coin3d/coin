/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "SoGLShaderObject.h"
#include "coindefs.h"

#include <assert.h>

#include "SoGLARBShaderObject.h"
#include "SoGLCgShaderObject.h"
#include "SoGLSLShaderObject.h"


static uint32_t shaderid = 0;

// *************************************************************************

SoGLShaderObject::SoGLShaderObject(const uint32_t cachecontext)
{
  this->isActiveFlag = TRUE;
  this->shadertype = VERTEX;
  this->paramsdirty = TRUE;
  this->glctx = cc_glglue_instance(cachecontext);
  this->cachecontext = cachecontext;
  this->id = ++shaderid;
}

const cc_glglue *
SoGLShaderObject::GLContext(void) const
{
  return this->glctx;
}

uint32_t
SoGLShaderObject::getCacheContext(void) const
{
  return this->cachecontext;
}

void
SoGLShaderObject::setShaderType(const ShaderType type)
{
  if (this->shadertype != type) {
    this->unload();
    this->shadertype = type;
  }
}

SoGLShaderObject::ShaderType
SoGLShaderObject::getShaderType(void) const
{
  return this->shadertype;
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

void
SoGLShaderObject::setParametersDirty(SbBool flag)
{
  this->paramsdirty = flag;
}

SbBool
SoGLShaderObject::getParametersDirty(void) const
{
  return this->paramsdirty;
}

void
SoGLShaderObject::updateCoinParameter(SoState * COIN_UNUSED_ARG(state), const SbName & COIN_UNUSED_ARG(name), SoShaderParameter * COIN_UNUSED_ARG(param), const int COIN_UNUSED_ARG(val))
{
}

uint32_t 
SoGLShaderObject::getShaderObjectId(void) const
{
  return this->id;
}
