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

#include "SoGLARBShaderObject.h"

#include <string.h>

#include <Inventor/errors/SoDebugError.h>
#include "SoGLARBShaderParameter.h"

// *************************************************************************

SoGLARBShaderObject::SoGLARBShaderObject(const cc_glglue * g)
  : SoGLShaderObject(g)
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
  int len = strlen(srcStr);

  this->target = isVertexShader() 
    ? GL_VERTEX_PROGRAM_ARB : GL_FRAGMENT_PROGRAM_ARB;

  this->unload();

  if (len == 0) return;

  glEnable(this->target);
  cc_glglue_glGenPrograms(this->glctx, 1, &this->arbProgramID);
  cc_glglue_glBindProgram(this->glctx, this->target, this->arbProgramID);
  cc_glglue_glProgramString(this->glctx, this->target, GL_PROGRAM_FORMAT_ASCII_ARB, len, srcStr);

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
    cc_glglue_glDeletePrograms(this->glctx, 1, &this->arbProgramID);
    this->arbProgramID = 0;
  }
}

SoShader::ShaderType
SoGLARBShaderObject::shaderType(void) const
{
  return SoShader::ARB_SHADER;
}

SoGLShaderParameter *
SoGLARBShaderObject::getParameter(int index, const char*, SoShader::ValueType)
{
  return new SoGLARBShaderParameter(this->target, index);
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
