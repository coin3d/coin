/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "SoGLSLShaderObject.h"

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/glue/glp.h>
#include <Inventor/system/gl.h>
#include "SoGLSLShaderParameter.h"
#include <assert.h>

static int32_t soglshaderobject_idcounter = 1;

// *************************************************************************

SoGLSLShaderObject::SoGLSLShaderObject(const uint32_t cachecontext)
  : SoGLShaderObject(cachecontext)
{
  this->programHandle = 0;
  this->shaderHandle = 0;
  this->isattached = FALSE;
  this->programid = 0;
}

SoGLSLShaderObject::~SoGLSLShaderObject()
{
  // make sure we don't detach, since the program might have been
  // destructed already. FIXME: investigate if not calling detach will
  // lead to memory leaks. pederb, 2006-10-17
  this->isattached = FALSE;
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
  this->setParametersDirty(TRUE);

  GLint flag;
  GLenum sType;

  switch (this->getShaderType()) {
  default:
    assert(0 &&" unknown shader type");
  case VERTEX:
    sType = GL_VERTEX_SHADER_ARB;
    break;
  case FRAGMENT:
    sType = GL_FRAGMENT_SHADER_ARB;
    break;
  case GEOMETRY:
    sType = GL_GEOMETRY_SHADER_EXT;
    break;
  }

  this->shaderHandle = this->glctx->glCreateShaderObjectARB(sType);
  this->programid = 0;

  if (this->shaderHandle == 0) return;
  this->programid = soglshaderobject_idcounter++;

  this->glctx->glShaderSourceARB(this->shaderHandle, 1, (const COIN_GLchar **)&srcStr, NULL);
  this->glctx->glCompileShaderARB(this->shaderHandle);

  if (SoGLSLShaderObject::didOpenGLErrorOccur(this->getShaderType())) {
    this->shaderHandle = 0;
    return;
  }

  this->glctx->glGetObjectParameterivARB(this->shaderHandle,
                                         GL_OBJECT_COMPILE_STATUS_ARB,
                                         &flag);
  SoGLSLShaderObject::printInfoLog(this->GLContext(), this->shaderHandle,
                                   this->getShaderType());

  if (!flag) this->shaderHandle = 0;
}

void
SoGLSLShaderObject::unload(void)
{
  this->detach();
  if (this->shaderHandle) { this->glctx->glDeleteObjectARB(this->shaderHandle); }
  this->shaderHandle = 0;
  this->programHandle = 0;
  this->programid = 0;
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
    this->isattached = TRUE;
  }
}

void
SoGLSLShaderObject::detach(void)
{
  if (this->isattached && this->programHandle && this->shaderHandle) {
    this->glctx->glDetachObjectARB(this->programHandle, this->shaderHandle);
    this->isattached = FALSE;
    this->programHandle = 0;
  }
}

SbBool
SoGLSLShaderObject::isAttached(void) const
{
  return this->isattached;
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
    case 0: s += "vertexShader "; break;
    case 1: s += "fragmentShader "; break;
    case 2: s += "geometryShader "; break;
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
    case 0: s += "vertexShader "; break;
    case 1: s += "fragmentShader "; break;
    case 2: s += "geometryShader "; break;
    default: ;// do nothing
    }
    SoDebugError::post("SoGLSLShaderObject::didOpenGLErrorOccur",
                       "%s error: '%s'",
                       s.getString(),
                       coin_glerror_string(glErr));
    retCode = TRUE;
    glErr = glGetError(); // FIXME: what is this for? 20050124 mortene.
  }
  return retCode;
}

#include <stdio.h>
#include <Inventor/SbName.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/actions/SoAction.h>
#include <stdio.h>

void 
SoGLSLShaderObject::updateCoinParameter(SoState * state, const SbName & name, SoShaderParameter * param, const int value)
{
  COIN_GLhandle pHandle = this->programHandle;
  if (pHandle) {
    const cc_glglue * glue = this->GLContext();
    
    // FIXME: set up a dict for the supported Coin variables 
    SoShaderParameter1i * p = (SoShaderParameter1i*) param;
    
    if (p) {
      if (p->value.getValue() != value) p->value = value;
    }
    else {
      GLint location = glue->glGetUniformLocationARB(pHandle,
                                                     (const COIN_GLchar *)name.getString());
      
#if 0
      fprintf(stderr,"action: %s, name: %s, loc: %d, handle: %p\n", 
              state->getAction()->getTypeId().getName().getString(),
              name.getString(), location, pHandle);
#endif
      if (location >= 0) {
        glue->glUniform1iARB(location, value);
      }
    }
  }
}
