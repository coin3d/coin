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

#include "SoGLShaderProgram.h"

#include <assert.h>

#include "SoGLShaderObject.h"
#include "SoGLARBShaderProgram.h"
#include "SoGLCgShaderProgram.h"
#include "SoGLSLShaderProgram.h"

// *************************************************************************

SoGLShaderProgram::SoGLShaderProgram(void)
{
  this->arbShaderProgram = new SoGLARBShaderProgram;
  this->cgShaderProgram = new SoGLCgShaderProgram;
  this->glslShaderProgram = new SoGLSLShaderProgram;
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
SoGLShaderProgram::removeShaderObject(SoGLShaderObject *shader)
{
  switch (shader->shaderType()) {
  case SoShader::ARB_SHADER:
    this->arbShaderProgram->removeShaderObject((SoGLARBShaderObject*)shader);
    break;
  case SoShader::CG_SHADER:
    this->cgShaderProgram->removeShaderObject((SoGLCgShaderObject*)shader);
    break;
  case SoShader::GLSL_SHADER:
    this->glslShaderProgram->removeShaderObject((SoGLSLShaderObject*)shader);
    break;
  default:
    assert(FALSE && "shaderType unknown!");
  }
}

void
SoGLShaderProgram::enable(const cc_glglue * g)
{
  this->arbShaderProgram->enable();
  this->cgShaderProgram->enable();
  this->glslShaderProgram->enable(g);
}

void
SoGLShaderProgram::disable(const cc_glglue * g)
{
  this->arbShaderProgram->disable();
  this->cgShaderProgram->disable();
  this->glslShaderProgram->disable(g);
}

void
SoGLShaderProgram::postShouldLink(void)
{
  // this is only neccessary for GLSLang
  this->glslShaderProgram->postShouldLink();
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
