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

#include "SoGLARBShaderProgram.h"

#include <string.h>

#include <Inventor/errors/SoDebugError.h>
#include "SoGLARBShaderObject.h"

// *************************************************************************

SoGLARBShaderProgram::SoGLARBShaderProgram(void)
{
  this->fragmentShader = NULL;
  this->vertexShader = NULL;
}

void
SoGLARBShaderProgram::addShaderObject(SoGLARBShaderObject *shaderObject)
{
  if (shaderObject->isVertexShader())
    this->vertexShader = shaderObject;
  else
    this->fragmentShader = shaderObject;
}

void
SoGLARBShaderProgram::removeShaderObject(SoGLARBShaderObject *shader)
{
  if (shader == NULL) return;

  if (shader->isVertexShader())
    this->vertexShader = NULL;
  else
    this->fragmentShader = NULL;
}

void
SoGLARBShaderProgram::enable(const cc_glglue * g)
{
  if (this->fragmentShader) this->fragmentShader->enable(g);
  if (this->vertexShader) this->vertexShader->enable(g);
}

void
SoGLARBShaderProgram::disable(const cc_glglue * g)
{
  if (this->fragmentShader) this->fragmentShader->disable(g);
  if (this->vertexShader) this->vertexShader->disable(g);
}

#if defined(SOURCE_HINT)
SbString
SoGLARBShaderProgram::getSourceHint(void) const
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
