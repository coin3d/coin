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

#include <Inventor/nodes/SoGLShaderProgram.h>

#include <assert.h>

#include "SoGLShaderObject.h"
#include "SoGLARBShader.h"
#include "SoGLCgShader.h"
#include "SoGLSLShader.h"

// *************************************************************************

SoGLShaderProgram::SoGLShaderProgram()
{
#if defined(SO_ARB_SHADER_SUPPORT)
  this->arbShaderProgram  = new SoGLARBShaderProgram;
#endif
#if defined(SO_CG_SHADER_SUPPORT)
  this->cgShaderProgram   = new SoGLCgShaderProgram;
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
  this->glslShaderProgram = new SoGLSLShaderProgram;
#endif
}

SoGLShaderProgram::~SoGLShaderProgram()
{
#if defined(SO_ARB_SHADER_SUPPORT)
  delete this->arbShaderProgram;
#endif
#if defined(SO_CG_SHADER_SUPPORT)
  delete this->cgShaderProgram;
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
  delete this->glslShaderProgram;
#endif
}

void SoGLShaderProgram::addShaderObject(SoGLShaderObject *shader)
{
  switch (shader->shaderType()) {
#if defined(SO_ARB_SHADER_SUPPORT)
    case SoGLShader::ARB_SHADER:
      this->arbShaderProgram->addShaderObject((SoGLARBShaderObject*)shader);
      break;
#endif
#if defined(SO_CG_SHADER_SUPPORT)
    case SoGLShader::CG_SHADER:
      this->cgShaderProgram->addShaderObject((SoGLCgShaderObject*)shader);
      break;
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
    case SoGLShader::GLSL_SHADER:
      this->glslShaderProgram->addShaderObject((SoGLSLShaderObject*)shader);
      break;
#endif
    default:
      assert(FALSE && "shaderType unknown!");
  }
}

void SoGLShaderProgram::removeShaderObject(SoGLShaderObject *shader)
{
  switch (shader->shaderType()) {
#if defined(SO_ARB_SHADER_SUPPORT)
    case SoGLShader::ARB_SHADER:
      this->arbShaderProgram->removeShaderObject((SoGLARBShaderObject*)shader);
      break;
#endif
#if defined(SO_CG_SHADER_SUPPORT)
    case SoGLShader::CG_SHADER:
      this->cgShaderProgram->removeShaderObject((SoGLCgShaderObject*)shader);
      break;
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
    case SoGLShader::GLSL_SHADER:
      this->glslShaderProgram->removeShaderObject((SoGLSLShaderObject*)shader);
      break;
#endif
    default:
      assert(FALSE && "shaderType unknown!");
  }
}

void SoGLShaderProgram::enable()
{
#if defined(SO_ARB_SHADER_SUPPORT)
  this->arbShaderProgram->enable();
#endif
#if defined(SO_CG_SHADER_SUPPORT)
  this->cgShaderProgram->enable();
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
  this->glslShaderProgram->enable();
#endif
}

void SoGLShaderProgram::disable()
{
#if defined(SO_ARB_SHADER_SUPPORT)
  this->arbShaderProgram->disable();
#endif
#if defined(SO_CG_SHADER_SUPPORT)
  this->cgShaderProgram->disable();
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
  this->glslShaderProgram->disable();
#endif
}

void SoGLShaderProgram::postShouldLink()
{
  // this is only neccessary for GLSLang
#if defined(SO_GLSL_SHADER_SUPPORT)
  this->glslShaderProgram->postShouldLink();
#endif
}

#if defined(SOURCE_HINT)
SbString SoGLShaderProgram::getSourceHint()
{
  SbString result;
  
#if defined(SO_ARB_SHADER_SUPPORT)
  result += this->arbShaderProgram->getSourceHint();
#endif
#if defined(SO_CG_SHADER_SUPPORT)
  result += this->cgShaderProgram->getSourceHint();
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
  result += this->glslShaderProgram->getSourceHint();
#endif
  return result;
}

#endif
