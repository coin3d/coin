#include <Inventor/nodes/SoGLShaderParameter.h>
#include "SoGLARBShader.h"
#include "SoGLCgShader.h"
#include "SoGLSLShader.h"
#include <assert.h>

SoGLShaderParameter::SoGLShaderParameter()
{
  this->type = SoGLShader::UNKNOWN_TYPE;
}

SbBool SoGLShaderParameter::isFloat() { return this->type==SoGLShader::FLOAT; }
SbBool SoGLShaderParameter::isFloat2(){ return this->type==SoGLShader::FLOAT2;}
SbBool SoGLShaderParameter::isFloat3(){ return this->type==SoGLShader::FLOAT3;}
SbBool SoGLShaderParameter::isFloat4(){ return this->type==SoGLShader::FLOAT4;}

SbBool SoGLShaderParameter::isTexture() 
{
  switch (this->type) {
    case SoGLShader::TEXTURE1D:
    case SoGLShader::TEXTURE2D:
    case SoGLShader::TEXTURE3D:
    case SoGLShader::TEXTURE_CUBE:
    case SoGLShader::TEXTURE_RECT:
      return TRUE;
    default:
      return FALSE;
  }
}

void SoGLShaderParameter::operator delete(void *obj)  
{
  switch (((SoGLShaderParameter*)obj)->shaderType()) {
#if defined(SO_ARB_SHADER_SUPPORT)
    case  SoGLShader::ARB_SHADER: 
      ::delete (SoGLARBShaderParameter *)obj; break;
#endif
#if defined(SO_CG_SHADER_SUPPORT)
    case   SoGLShader::CG_SHADER:
      ::delete (SoGLCgShaderParameter *)obj;  break;
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
    case SoGLShader::GLSL_SHADER: 
      ::delete (SoGLSLShaderParameter *)obj;  break;
#endif
             default: assert(FALSE && "shaderType unknown!");
  }
}

void SoGLShaderParameter::operator delete[](void *obj)  
{
  switch (((SoGLShaderParameter*)obj)->shaderType()) {
#if defined(SO_ARB_SHADER_SUPPORT)
    case  SoGLShader::ARB_SHADER:
      ::delete [] (SoGLARBShaderParameter *)obj; break;
#endif
#if defined(SO_CG_SHADER_SUPPORT)
    case   SoGLShader::CG_SHADER:
      ::delete [] (SoGLCgShaderParameter *)obj;  break;
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
    case SoGLShader::GLSL_SHADER:
      ::delete [] (SoGLSLShaderParameter *)obj;  break;
#endif
             default: assert(FALSE && "shaderType unknown!");
  }
}
