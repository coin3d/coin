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

SoGLARBShaderObject::SoGLARBShaderObject(void)
{
  this->arbProgramID = 0;
}

SoGLARBShaderObject::~SoGLARBShaderObject()
{
}

SbBool
SoGLARBShaderObject::isLoaded(const cc_glglue * g) const
{
  return cc_glglue_glIsProgram(g, this->arbProgramID);
}

void
SoGLARBShaderObject::load(const cc_glglue * g, const char * srcStr)
{
  int len = strlen(srcStr);

  this->target = isVertexShader() 
    ? GL_VERTEX_PROGRAM_ARB : GL_FRAGMENT_PROGRAM_ARB;

  this->unload(g);

  if (len == 0) return;

  glEnable(this->target);
  cc_glglue_glGenPrograms(g, 1, &this->arbProgramID);
  cc_glglue_glBindProgram(g, this->target, this->arbProgramID);
  cc_glglue_glProgramString(g, this->target, GL_PROGRAM_FORMAT_ASCII_ARB, len, srcStr);

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
SoGLARBShaderObject::unload(const cc_glglue * g)
{
  if (cc_glglue_glIsProgram(g, this->arbProgramID)) {
    cc_glglue_glDeletePrograms(g, 1, &this->arbProgramID);
    this->arbProgramID = 0;
  }
}

SoGLShader::ShaderType
SoGLARBShaderObject::shaderType() const
{
  return SoGLShader::ARB_SHADER;
}

SoGLShaderParameter *
SoGLARBShaderObject::getParameter(int index, const char*, SoGLShader::ValueType)
{
  return new SoGLARBShaderParameter(this->target, index);
}

void
SoGLARBShaderObject::enable(const cc_glglue * g)
{
  if (this->isActive(g)) {
    cc_glglue_glBindProgram(g, this->target, this->arbProgramID);
    glEnable(this->target);
  }
}

void
SoGLARBShaderObject::disable(const cc_glglue * g)
{
  if (this->isActive(g)) glDisable(this->target);
}
