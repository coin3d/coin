#include "SoGLShaderObject.h"

#include <assert.h>

#include "SoGLARBShader.h"
#include "SoGLCgShader.h"
#include "SoGLSLShader.h"

SoGLShaderObject::SoGLShaderObject()
{
  this->isActiveFlag       = TRUE;
  this->isVertexShaderFlag = TRUE;
}

void SoGLShaderObject::operator delete(void *obj)
{
  switch (((SoGLShaderObject *)obj)->shaderType()) {
#if defined (SO_ARB_SHADER_SUPPORT)
    case  SoGLShader::ARB_SHADER: ::delete (SoGLARBShaderObject *)obj; break;
#endif
#if defined(SO_CG_SHADER_SUPPORT)
    case   SoGLShader::CG_SHADER: ::delete (SoGLCgShaderObject *)obj;  break;
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
    case SoGLShader::GLSL_SHADER: ::delete (SoGLSLShaderObject*)obj;   break;
#endif
             default: assert(FALSE && "shaderType unknown!");
  }
}

void SoGLShaderObject::operator delete[](void *obj)  
{
  switch (((SoGLShaderObject *)obj)->shaderType()) {
#if defined(SO_ARB_SHADER_SUPPORT)
    case  SoGLShader::ARB_SHADER: ::delete [](SoGLARBShaderObject *)obj; break;
#endif
#if defined(SO_CG_SHADER_SUPPORT)
    case   SoGLShader::CG_SHADER: ::delete [](SoGLCgShaderObject *)obj;  break;
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
    case  SoGLShader::GLSL_SHADER: ::delete [](SoGLSLShaderObject*)obj;  break;
#endif
             default: assert(FALSE && "shaderType unknown!");
  }
}

void SoGLShaderObject::setIsVertexShader(SbBool flag)
{
  if (this->isVertexShaderFlag != flag) {
    unload();
    this->isVertexShaderFlag = flag;
  }
}

SbBool SoGLShaderObject::isVertexShader() const
{
  return this->isVertexShaderFlag;
}

void SoGLShaderObject::setIsActive(SbBool flag)
{
  this->isActiveFlag = flag;
}

SbBool SoGLShaderObject::isActive() const
{
  return (!this->isLoaded()) ?  false : this->isActiveFlag;
}


