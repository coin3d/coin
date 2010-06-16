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

#include "SoGLARBShaderObject.h"

#include <string.h>

#include <Inventor/errors/SoDebugError.h>
#include "SoGLARBShaderParameter.h"

// *************************************************************************

SoGLARBShaderObject::SoGLARBShaderObject(const uint32_t cachecontext)
  : SoGLShaderObject(cachecontext)
{
  this->arbProgramID = 0;
}

SoGLARBShaderObject::~SoGLARBShaderObject()
{
}

SbBool
SoGLARBShaderObject::isLoaded(void) const
{
  return cc_glglue_glIsProgram(this->glctx, this->arbProgramID);
}

void
SoGLARBShaderObject::load(const char * srcStr)
{
  const size_t len = strlen(srcStr);
  
  this->target = this->getShaderType() == VERTEX 
    ? GL_VERTEX_PROGRAM_ARB : GL_FRAGMENT_PROGRAM_ARB;

  this->unload();

  if (len == 0) return;

  glEnable(this->target);
  cc_glglue_glGenPrograms(this->glctx, 1, &this->arbProgramID);
  cc_glglue_glBindProgram(this->glctx, this->target, this->arbProgramID);
  cc_glglue_glProgramString(this->glctx, this->target, GL_PROGRAM_FORMAT_ASCII_ARB, (GLsizei)len, srcStr);

  if (glGetError() == GL_INVALID_OPERATION) {
    GLint errorPos;
    const GLubyte *errorString;

    glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorPos);
    errorString = glGetString(GL_PROGRAM_ERROR_STRING_ARB);
    SoDebugError::post("SoGLARBShaderObject::load",
                       "Error at position: %d (%s)",
                       errorPos, errorString);
  }

  glDisable(this->target);
}

void
SoGLARBShaderObject::unload(void)
{
  if (cc_glglue_glIsProgram(this->glctx, this->arbProgramID)) {
    // FIXME: make sure this is only called when in the correct, valid
    // GL context. 20050120 mortene.
    cc_glglue_glDeletePrograms(this->glctx, 1, &this->arbProgramID);
    this->arbProgramID = 0;
  }
}

SoShader::Type
SoGLARBShaderObject::shaderType(void) const
{
  return SoShader::ARB_SHADER;
}

SoGLShaderParameter *
SoGLARBShaderObject::getNewParameter(void) const
{
  return new SoGLARBShaderParameter();
}

void
SoGLARBShaderObject::enable(void)
{
  if (this->isActive()) {
    cc_glglue_glBindProgram(this->glctx, this->target, this->arbProgramID);
    glEnable(this->target);
  }
}

void
SoGLARBShaderObject::disable(void)
{
  if (this->isActive()) glDisable(this->target);
}
