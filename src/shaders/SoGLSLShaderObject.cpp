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

#include "SoGLSLShaderObject.h"

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoGL.h>
#include "SoGLSLShaderParameter.h"

// *************************************************************************

SoGLSLShaderObject::SoGLSLShaderObject(const cc_glglue * g)
  : SoGLShaderObject(g)
{
  this->programHandle = 0;
  this->shaderHandle = 0;
}

SoGLSLShaderObject::~SoGLSLShaderObject()
{
  this->unload();
}

// *************************************************************************

SoShader::Type
SoGLSLShaderObject::shaderType(void) const
{
  return SoShader::GLSL_SHADER;
}

SbBool
SoGLSLShaderObject::isLoaded(void) const
{
  return (this->shaderHandle != 0);
}

void
SoGLSLShaderObject::load(const char* srcStr)
{
  this->unload();
  GLint flag;
  GLenum sType;

  sType = (isVertexShader()) ? GL_VERTEX_SHADER_ARB : GL_FRAGMENT_SHADER_ARB;

  this->shaderHandle = this->glctx->glCreateShaderObjectARB(sType);

  if (this->shaderHandle == 0) return;

  this->glctx->glShaderSourceARB(this->shaderHandle, 1, (const COIN_GLchar **)&srcStr, NULL);
  this->glctx->glCompileShaderARB(this->shaderHandle);

  if (SoGLSLShaderObject::didOpenGLErrorOccur((isVertexShader()) ? 1 : 2)) {
    this->shaderHandle = 0;
    return;
  }

  this->glctx->glGetObjectParameterivARB(this->shaderHandle,
                                         GL_OBJECT_COMPILE_STATUS_ARB,
                                         &flag);
  SoGLSLShaderObject::printInfoLog(this->GLContext(), this->shaderHandle,
                                   this->isVertexShader() ? 1 : 2);

  if (!flag) this->shaderHandle = 0;
}

void
SoGLSLShaderObject::unload(void)
{
  this->detach();
  if (this->shaderHandle) { this->glctx->glDeleteObjectARB(this->shaderHandle); }
  this->shaderHandle = 0;
  this->programHandle = 0;
}

SoGLShaderParameter *
SoGLSLShaderObject::getNewParameter(void) const
{
  return new SoGLSLShaderParameter();
}

// *************************************************************************

void
SoGLSLShaderObject::attach(COIN_GLhandle programHandle)
{
  if (programHandle <= 0 || this->programHandle == programHandle) return;

  detach();

  if (this->shaderHandle) {
    this->programHandle = programHandle;
    this->glctx->glAttachObjectARB(this->programHandle, this->shaderHandle);
  }
}

void
SoGLSLShaderObject::detach(void)
{
  if (this->programHandle && this->shaderHandle) {
    this->glctx->glDetachObjectARB(this->programHandle, this->shaderHandle);
  }
}

void
SoGLSLShaderObject::printInfoLog(const cc_glglue * g, COIN_GLhandle handle, int objType)
{
  GLint length = 0;

  g->glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);

  if (length > 1) {
    COIN_GLchar *infoLog = new COIN_GLchar[length];
    GLsizei charsWritten = 0;
    g->glGetInfoLogARB(handle, length, &charsWritten, infoLog);
    SbString s("GLSL");
    switch (objType) {
    case 0: s += "program "; break;
    case 1: s += "vertexShader "; break;
    case 2: s += "fragmentShader "; break;
    default: ;// do nothing 
    }
    SoDebugError::postInfo("SoGLSLShaderObject::printInfoLog",
                           "%s log: '%s'",
                           s.getString(), infoLog);
    delete [] infoLog;
  }
}

SbBool
SoGLSLShaderObject::didOpenGLErrorOccur(int objType)
{
  // FIXME: improve on this function -- catching up on gl errors
  // should perhaps better be part of the cc_glglue
  // interface. 20050124 mortene.

  GLenum glErr;
  SbBool retCode = FALSE;

  glErr = glGetError();
  while (glErr != GL_NO_ERROR) {
    SbString s("GLSL");
    switch (objType) {
    case 0: s += "program "; break;
    case 1: s += "vertexShader "; break;
    case 2: s += "fragmentShader "; break;
    default: ;// do nothing 
    }
    SoDebugError::post("SoGLSLShaderObject::didOpenGLErrorOccur",
                       "%s error: '%s'",
                       s.getString(),
                       sogl_glerror_string(glErr).getString());
    retCode = TRUE;
    glErr = glGetError(); // FIXME: what is this for? 20050124 mortene.
  }
  return retCode;
}
