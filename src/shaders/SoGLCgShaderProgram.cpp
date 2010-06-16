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

#include "SoGLCgShaderProgram.h"

#include <assert.h>
#include <stdlib.h>

#include "SoGLCgShaderObject.h"

// *************************************************************************

SoGLCgShaderProgram::SoGLCgShaderProgram(void)
{
  this->fragmentShader = NULL;
  this->vertexShader = NULL;
}

SoGLCgShaderProgram::~SoGLCgShaderProgram()
{
}

void
SoGLCgShaderProgram::addShaderObject(SoGLCgShaderObject * shaderObject)
{
  if (shaderObject->getShaderType() == SoGLShaderObject::VERTEX) {
    this->vertexShader = shaderObject;
  }
  else {
    assert(shaderObject->getShaderType() == SoGLShaderObject::FRAGMENT);
    this->fragmentShader = shaderObject;
  }
}

void
SoGLCgShaderProgram::removeShaderObjects(void)
{
  this->vertexShader = NULL;
  this->fragmentShader = NULL;
}

void
SoGLCgShaderProgram::enable(void)
{
  if (this->fragmentShader) this->fragmentShader->enable();
  if (this->vertexShader) this->vertexShader->enable();
}

void
SoGLCgShaderProgram::disable(void)
{
  if (this->fragmentShader) this->fragmentShader->disable();
  if (this->vertexShader) this->vertexShader->disable();
}

#if defined(SOURCE_HINT)
SbString
SoGLCgShaderProgram::getSourceHint(void) const
{
  SbString result;

  if (this->fragmentShader && this->fragmentShader->isActive()) {
    SbString str = this->fragmentShader->sourceHint;
    if (str.getLength() > 0) str += " ";
    result += str;
  }
  if (this->vertexShader && this->vertexShader->isActive()) {
    SbString str = this->vertexShader->sourceHint;
    if (str.getLength() > 0) str += " ";
    result += str;
  }
  return result;
}
#endif
