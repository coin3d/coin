#if defined(SO_ARB_SHADER_SUPPORT)

#include "SoGLARBShader.h"
#include <iostream>

#include <GL/glext.h>

#ifdef _WIN32
#ifndef SO_GLSL_SHADER_SUPPORT
PFNGLGETACTIVEUNIFORMARBPROC        glGetActiveUniformARB;
PFNGLGETUNIFORMLOCATIONARBPROC      glGetUniformLocationARB;
#endif

PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
PFNGLISPROGRAMARBPROC               glIsProgramARB;
PFNGLGENPROGRAMSARBPROC             glGenProgramsARB;
PFNGLBINDPROGRAMARBPROC             glBindProgramARB;
PFNGLPROGRAMSTRINGARBPROC           glProgramStringARB;
PFNGLDELETEPROGRAMSARBPROC          glDeleteProgramsARB;
#endif

/*****************************************************************************
 ***                      SoGLARBShaderParameter implementation            ***
 *****************************************************************************/

SoGLARBShaderParameter::SoGLARBShaderParameter(GLenum theTarget, GLuint index)
{
  this->target     = theTarget;
  this->identifier = index;
}

SoGLARBShaderParameter::~SoGLARBShaderParameter()
{
}

SoGLShader::ShaderType SoGLARBShaderParameter::shaderType() const
{
  return SoGLShader::ARB_SHADER;
}

void SoGLARBShaderParameter::set1f(const float value)
{
  glProgramLocalParameter4fARB(this->target, this->identifier,
			       value, value, value, value);
}

void SoGLARBShaderParameter::set2f(const float *value)
{
  glProgramLocalParameter4fARB(this->target, this->identifier,
			       value[0], value[1], value[0], value[0]);
}

void SoGLARBShaderParameter::set3f(const float *value)
{
  glProgramLocalParameter4fARB(this->target, this->identifier,
			       value[0], value[1], value[2], value[0]);
}

void SoGLARBShaderParameter::set4f(const float *value)
{
  glProgramLocalParameter4fARB(this->target, this->identifier,
			       value[0], value[1], value[2], value[3]);
}

/****************************************************************************
 ***                     SoGLARBShaderObject implementation               ***
 ****************************************************************************/

SoGLARBShaderObject::SoGLARBShaderObject()
{
#ifdef _WIN32
# define PADDR(functype,funcname) \
  (funcname = (functype) wglGetProcAddress( #funcname ))

  static SbBool isInitialized = FALSE;

  if (!isInitialized) {
    isInitialized = TRUE;

#ifndef SO_GLSL_SHADER_SUPPORT
    PADDR(PFNGLGETACTIVEUNIFORMARBPROC,        glGetActiveUniformARB);
    PADDR(PFNGLGETUNIFORMLOCATIONARBPROC,      glGetUniformLocationARB);
#endif

    PADDR(PFNGLPROGRAMLOCALPARAMETER4FARBPROC, glProgramLocalParameter4fARB);
    PADDR(PFNGLISPROGRAMARBPROC,               glIsProgramARB);
    PADDR(PFNGLGENPROGRAMSARBPROC,             glGenProgramsARB);
    PADDR(PFNGLBINDPROGRAMARBPROC,             glBindProgramARB);
    PADDR(PFNGLPROGRAMSTRINGARBPROC,           glProgramStringARB);
    PADDR(PFNGLDELETEPROGRAMSARBPROC,          glDeleteProgramsARB);
  }
# undef PADDR
#endif
  this->arbProgramID = 0;
}

SoGLARBShaderObject::~SoGLARBShaderObject()
{
}

SbBool SoGLARBShaderObject::isLoaded() const
{
  return glIsProgramARB(this->arbProgramID);
}

void SoGLARBShaderObject::load(const char* srcStr)
{
  int len = strlen(srcStr);

  this->target = isVertexShader() 
    ? GL_VERTEX_PROGRAM_ARB : GL_FRAGMENT_PROGRAM_ARB;

  this->unload();

  if (len == 0) return;

  glEnable(this->target);
  glGenProgramsARB(1, &this->arbProgramID);
  glBindProgramARB(this->target, this->arbProgramID);
  glProgramStringARB(this->target, GL_PROGRAM_FORMAT_ASCII_ARB, len, srcStr);

  if (glGetError() == GL_INVALID_OPERATION) {
    GLint          errorPos;
    const GLubyte *errorString;
  
    glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorPos);
    errorString = glGetString(GL_PROGRAM_ERROR_STRING_ARB);
    std::cerr << "Error at position: " << errorPos << std::endl;
    std::cerr << errorString << std::endl;
  }
  
  glDisable(this->target);
}

void SoGLARBShaderObject::unload()
{
  if (glIsProgramARB(this->arbProgramID)) {
    glDeleteProgramsARB(1, &this->arbProgramID);
    this->arbProgramID = 0;
  }
}

SoGLShader::ShaderType SoGLARBShaderObject::shaderType() const
{
  return SoGLShader::ARB_SHADER;
}

SoGLShaderParameter* SoGLARBShaderObject
::getParameter(int index, const char* name, SoGLShader::ValueType type)
{
  return new SoGLARBShaderParameter(this->target, index);
}

void SoGLARBShaderObject::enable()
{
  if (this->isActive()) {
    glBindProgramARB(this->target, this->arbProgramID);
    glEnable(this->target);
  }
}

void SoGLARBShaderObject::disable()
{
  if (this->isActive()) glDisable(this->target);
}

/***************************************************************************
 ***                   SoGLARBShaderProgram implementation               ***
 ***************************************************************************/

SoGLARBShaderProgram::SoGLARBShaderProgram()
{
  this->fragmentShader = NULL;
  this->vertexShader   = NULL;
}

void SoGLARBShaderProgram::addShaderObject(SoGLARBShaderObject *shaderObject)
{
 if (shaderObject->isVertexShader())
    this->vertexShader = shaderObject;
  else
    this->fragmentShader = shaderObject;
}

void SoGLARBShaderProgram::removeShaderObject(SoGLARBShaderObject *shader)
{
  if (shader == NULL) return;
  
  if (shader->isVertexShader())
    this->vertexShader = NULL;
  else
    this->fragmentShader = NULL;
}

void SoGLARBShaderProgram::enable()
{
  if (this->fragmentShader) this->fragmentShader->enable();
  if (this->vertexShader)   this->vertexShader->enable();
}

void SoGLARBShaderProgram::disable()
{
  if (this->fragmentShader) this->fragmentShader->disable();
  if (this->vertexShader)   this->vertexShader->disable();
}

#if defined(SOURCE_HINT)
SbString SoGLARBShaderProgram::getSourceHint() const
{
  SbString result;
  
  if (this->fragmentShader && this->fragmentShader->isActive()) {
    SbString str = this->fragmentShader->sourceHint;
    if (str.getLength() > 0) str += " ";
    result += str;
  }
  if (this->vertexShader && this->vertexShader->isActive()) {
    SbString str = this->vertexShader->sourceHint;
    if (str.getLength() > 0) str += " ";
    result += str;
  }
  return result;
}
#endif

#endif /* SO_ARB_SHADER_SUPPORT */
