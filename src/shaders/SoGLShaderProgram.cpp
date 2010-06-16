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

#include "SoGLShaderProgram.h"

#include <assert.h>

#include "SoGLShaderObject.h"
#include "SoGLARBShaderProgram.h"
#include "SoGLCgShaderProgram.h"
#include "SoGLSLShaderProgram.h"
#include <Inventor/elements/SoGLCacheContextElement.h>

// *************************************************************************

SoGLShaderProgram::SoGLShaderProgram(void)
{
  this->arbShaderProgram = new SoGLARBShaderProgram;
  this->cgShaderProgram = new SoGLCgShaderProgram;
  this->glslShaderProgram = new SoGLSLShaderProgram;

  this->isenabled = FALSE;
  this->enablecb = NULL;
  this->enablecbclosure = NULL;
}

SoGLShaderProgram::~SoGLShaderProgram()
{
  delete this->arbShaderProgram;
  delete this->cgShaderProgram;
  delete this->glslShaderProgram;
}

void
SoGLShaderProgram::addShaderObject(SoGLShaderObject *shader)
{
  this->objectids.append(shader->getShaderObjectId());
  switch (shader->shaderType()) {
  case SoShader::ARB_SHADER:
    this->arbShaderProgram->addShaderObject((SoGLARBShaderObject*)shader);
    break;
  case SoShader::CG_SHADER:
    this->cgShaderProgram->addShaderObject((SoGLCgShaderObject*)shader);
    break;
  case SoShader::GLSL_SHADER:
    this->glslShaderProgram->addShaderObject((SoGLSLShaderObject*)shader);
    break;
  default:
    assert(FALSE && "shaderType unknown!");
  }
}

void
SoGLShaderProgram::removeShaderObjects(void)
{
  this->arbShaderProgram->removeShaderObjects();
  this->cgShaderProgram->removeShaderObjects();
  this->glslShaderProgram->removeShaderObjects();
  this->glslShaderProgram->removeProgramParameters();
  this->objectids.truncate(0);
}

void
SoGLShaderProgram::enable(SoState * state)
{
  const uint32_t cachecontext = SoGLCacheContextElement::get(state);
  const cc_glglue * glctx = cc_glglue_instance(cachecontext);

  this->arbShaderProgram->enable();
  this->cgShaderProgram->enable();
  this->glslShaderProgram->enable(glctx);

  this->isenabled = TRUE;
  if (this->enablecb) {
    this->enablecb(this->enablecbclosure, state, TRUE);
  }
}

void
SoGLShaderProgram::disable(SoState * state)
{
  const uint32_t cachecontext = SoGLCacheContextElement::get(state);
  const cc_glglue * glctx = cc_glglue_instance(cachecontext);

  this->arbShaderProgram->disable();
  this->cgShaderProgram->disable();
  this->glslShaderProgram->disable(glctx);

  this->isenabled = FALSE;
  if (this->enablecb) {
    this->enablecb(this->enablecbclosure, state, FALSE);
  }
}

SbBool
SoGLShaderProgram::isEnabled(void) const
{
  return this->isenabled;
}

void
SoGLShaderProgram::setEnableCallback(SoShaderProgramEnableCB * cb,
                                     void * closure)
{
  this->enablecb = cb;
  this->enablecbclosure = closure;
}

void
SoGLShaderProgram::updateCoinParameter(SoState * state, const SbName & name, const int value)
{
  if (this->glslShaderProgram) {
    SbBool enabled = this->isenabled;
    if (!enabled) this->enable(state);
    this->glslShaderProgram->updateCoinParameter(state, name, value);
    if (!enabled) this->disable(state);
  }
}

void
SoGLShaderProgram::addProgramParameter(int name, int value)
{
  if (this->glslShaderProgram) {
    this->glslShaderProgram->addProgramParameter(name, value);
  }
}

void
SoGLShaderProgram::getShaderObjectIds(SbList <uint32_t> & ids) const
{
  ids = this->objectids;
}

uint32_t
SoGLShaderProgram::getGLSLShaderProgramHandle(SoState * state) const
{
  const uint32_t cachecontext = SoGLCacheContextElement::get(state);
  const cc_glglue * glctx = cc_glglue_instance(cachecontext);

  return this->glslShaderProgram->getProgramHandle(glctx);
}

SbBool
SoGLShaderProgram::glslShaderProgramLinked(void) const
{
  if (this->glslShaderProgram) {
    return this->glslShaderProgram->neededLinking();
  }
  return FALSE;
}

#if defined(SOURCE_HINT)
SbString
SoGLShaderProgram::getSourceHint(void)
{
  SbString result;

  result += this->arbShaderProgram->getSourceHint();
  result += this->cgShaderProgram->getSourceHint();
  result += this->glslShaderProgram->getSourceHint();
  return result;
}
#endif
