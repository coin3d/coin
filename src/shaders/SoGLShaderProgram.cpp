/*** -*- Mode: C++ -*- *****************************************************
 *
 *
 ***************************************************************************/

#include <Inventor/nodes/SoGLShaderProgram.h>
#include "SoGLShaderObject.h"
#include "SoGLARBShader.h"
#include "SoGLCgShader.h"
#include "SoGLSLShader.h"
#include <assert.h>
#include <iostream>

/* **************************************************************************
 * ***                    SoGLShaderProgram implementation                ***
 * **************************************************************************/

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
