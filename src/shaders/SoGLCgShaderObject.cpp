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

#include "SoGLCgShaderObject.h"

#include <assert.h>

#include <Inventor/errors/SoDebugError.h>
#include "SoGLCgShaderParameter.h"

// *************************************************************************

CGcontext SoGLCgShaderObject::cgContext = NULL;
int SoGLCgShaderObject::instanceCount = 0;

SoGLCgShaderObject::SoGLCgShaderObject(const cc_glglue * g)
  : SoGLShaderObject(g)
{
  SoGLCgShaderObject::instanceCount++;
  this->cgProgram = NULL;
}

SoGLCgShaderObject::~SoGLCgShaderObject()
{
  this->unload();
  SoGLCgShaderObject::destroyCgContext();
}

SoShaders::ShaderType
SoGLCgShaderObject::shaderType() const
{
  return SoShaders::CG_SHADER;
}

SoGLShaderParameter *
SoGLCgShaderObject::getParameter(int, const char* name, SoShaders::ValueType type)
{
  return new SoGLCgShaderParameter(this, name, type);
}

SbBool
SoGLCgShaderObject::isLoaded() const
{
  return glue_cgIsProgram(this->cgProgram);
}

void
SoGLCgShaderObject::load(const char* sourceString)
{
  CGerror errorCode = CG_NO_ERROR;
 
  SoGLCgShaderObject::ensureCgContext();

  this->unload();

  this->cgProfile = getProfile();
  //cgGLSetOptimalOptions(this->cgProfile);

  glue_cgSetErrorCallback(NULL);
  glue_cgGetError(); // remove last error from stack

  this->cgProgram =
    glue_cgCreateProgram(SoGLCgShaderObject::cgContext,
                         CG_SOURCE, 
                         sourceString,
                         this->cgProfile, 
                         "main", // entry's function name
                         NULL); // argument names
  errorCode = glue_cgGetError();

  if (errorCode == CG_NO_ERROR) {
    glue_cgGLLoadProgram(this->cgProgram);
    errorCode = glue_cgGetError();
  }

  if (errorCode != CG_NO_ERROR) {
    this->unload();
    SoGLCgShaderObject::printError(errorCode, SoGLCgShaderObject::cgContext);
  }

  glue_cgSetErrorCallback(SoGLCgShaderObject::cgErrorCallback);

  return;
}

void
SoGLCgShaderObject::unload()
{
  if (glue_cgIsProgram(this->cgProgram)) {
    glue_cgDestroyProgram(this->cgProgram);
    this->cgProgram = NULL;
  }
}


void
SoGLCgShaderObject::enable()
{
  if (glue_cgIsProgram(this->cgProgram)) {
    glue_cgGLBindProgram(this->cgProgram);
    glue_cgGLEnableProfile(this->cgProfile);
  }
}

void
SoGLCgShaderObject::disable()
{
  if (glue_cgIsProgram(this->cgProgram)) glue_cgGLDisableProfile(this->cgProfile);
}

CGprofile
SoGLCgShaderObject::getProfile(void) const
{
  CGprofile result = isVertexShader() ? CG_PROFILE_ARBVP1 : CG_PROFILE_ARBFP1;

  if (!glue_cgGLIsProfileSupported(result)) {
    SoDebugError::postWarning("SoGLCgShaderObject::getProfile",
                              "profile '%s' is not supported",
                              glue_cgGetProfileString(result));

    if (isVertexShader())
      result = glue_cgGLGetLatestProfile(CG_GL_VERTEX);
    else
      result = glue_cgGLGetLatestProfile(CG_GL_FRAGMENT);

    SoDebugError::postWarning("SoGLCgShaderObject::getProfile",
                              "'%s' will be used instead",
                              glue_cgGetProfileString(result));
  }
  return result;
}

// --- static methods -------------------------------------------------------

void
SoGLCgShaderObject::printError(CGerror error, CGcontext context)
{
#if defined(SOURCE_HINT) && 0
  std::cerr << "+++ " << this->sourceHint.getString() << " +++" << std::endl;
#endif
  SoDebugError::post("SoGLCgShaderObject::printError",
                     "'%s'", glue_cgGetErrorString(error));
  SoDebugError::post("SoGLCgShaderObject::printError",
                     "'%s'", glue_cgGetLastListing(context));
}

void
SoGLCgShaderObject::cgErrorCallback(void)
{
  CGerror lastError = glue_cgGetError();

  if(lastError) printError(lastError, SoGLCgShaderObject::cgContext);
}

void
SoGLCgShaderObject::ensureCgContext()
{
  if (!glue_cgIsContext(SoGLCgShaderObject::cgContext)) {
    SoGLCgShaderObject::cgContext = glue_cgCreateContext();
    glue_cgSetErrorCallback(SoGLCgShaderObject::cgErrorCallback);
  }
}

void
SoGLCgShaderObject::destroyCgContext()
{
  SoGLCgShaderObject::instanceCount--;

  if (SoGLCgShaderObject::instanceCount > 0) return;
  if (glue_cgIsContext(SoGLCgShaderObject::cgContext)) {
    glue_cgDestroyContext(SoGLCgShaderObject::cgContext);
  }
}
