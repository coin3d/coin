#if defined(SO_GLSL_SHADER_SUPPORT)

#include "SoGLSLShader.h"
#include <GL/glu.h>
#include <iostream>

#ifdef _WIN32
PFNGLGETACTIVEUNIFORMARBPROC     glGetActiveUniformARB;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB;

PFNGLUNIFORM1FARBPROC            glUniform1fARB;
PFNGLUNIFORM2FARBPROC            glUniform2fARB;
PFNGLUNIFORM3FARBPROC            glUniform3fARB;
PFNGLUNIFORM4FARBPROC            glUniform4fARB;

PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB;
PFNGLDETACHOBJECTARBPROC         glDetachObjectARB;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB;

PFNGLSHADERSOURCEARBPROC         glShaderSourceARB;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB;

PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB;
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB;

PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
#endif

/***************************************************************************
 ***                  SoGLSLShaderParameter implementation               ***
 ***************************************************************************/

SoGLSLShaderParameter::SoGLSLShaderParameter(GLhandleARB           program,
					     const char*           theName, 
					     SoGLShader::ValueType)
{
  this->location = glGetUniformLocationARB(program,(const GLcharARB *)theName);
  if (this->location == -1)
    this->type = SoGLShader::UNKNOWN_TYPE;
  else {
    GLsizei   length, size;
    GLenum    type;
    GLcharARB name[128];
    
    glGetActiveUniformARB(program,this->location,128,&length,&size,&type,name);
    this->type = SoGLSLShaderParameter::getParameterTypeFor(type);
    if (this->type == SoGLShader::UNKNOWN_TYPE) 
      this->location = -1;
    else
      this->name = theName;
  }
}

SoGLSLShaderParameter::~SoGLSLShaderParameter()
{
}

SoGLShader::ShaderType SoGLSLShaderParameter::shaderType() const
{ 
  return SoGLShader::GLSL_SHADER;
}

void SoGLSLShaderParameter::set1f(const float value, const char*, const int)
{
  if (this->isReferenced() && isFloat()) glUniform1fARB(this->location, value);
}

void SoGLSLShaderParameter::set2f(const float *value, const char*, const int)
{
  if (this->isReferenced() && isFloat2()) 
    glUniform2fARB(this->location, value[0], value[1]);
}

void SoGLSLShaderParameter::set3f(const float *value, const char*, const int)
{
  if (this->isReferenced() && isFloat3())
    glUniform3fARB(this->location, value[0], value[1], value[2]);
}

void SoGLSLShaderParameter::set4f(const float *value, const char*, const int)
{
  if (this->isReferenced() && isFloat4()) 
    glUniform4fARB(this->location, value[0], value[1],value[2],value[3]);
}

// --- static methods ------------------------------------------------------

SoGLShader::ValueType SoGLSLShaderParameter::getParameterTypeFor(GLenum type)
{
  switch (type) {
    case GL_FLOAT:           return SoGLShader::FLOAT;
    case GL_FLOAT_VEC2_ARB:  return SoGLShader::FLOAT2;
    case GL_FLOAT_VEC3_ARB:  return SoGLShader::FLOAT3;
    case GL_FLOAT_VEC4_ARB:  return SoGLShader::FLOAT4;
    case GL_FLOAT_MAT2_ARB:  return SoGLShader::FLOAT_MATRIX2;
    case GL_FLOAT_MAT3_ARB:  return SoGLShader::FLOAT_MATRIX3;
    case GL_FLOAT_MAT4_ARB:  return SoGLShader::FLOAT_MATRIX4;
    default:
      std::cerr << "cannot map GLtype to ValueType" << std::endl;
      return SoGLShader::UNKNOWN_TYPE;
  }
}

/***************************************************************************
 ***                    SoGLSLShaderObject implementation                ***
 ***************************************************************************/

SoGLShader::ShaderType SoGLSLShaderObject::shaderType() const
{
  return SoGLShader::GLSL_SHADER;
}

SbBool SoGLSLShaderObject::isLoaded() const
{
  return (this->shaderHandle != 0);
}

void SoGLSLShaderObject::load(const char* srcStr)
{
  this->unload();
  GLint flag;
  GLenum sType;

  sType = (isVertexShader()) ? GL_VERTEX_SHADER_ARB : GL_FRAGMENT_SHADER_ARB;
  
  this->shaderHandle = glCreateShaderObjectARB(sType);

  if (this->shaderHandle == 0) return;
  
  glShaderSourceARB(this->shaderHandle, 1, (const GLcharARB **)&srcStr, NULL);
  glCompileShaderARB(this->shaderHandle);

  if (SoGLSLShaderObject::didOpenGLErrorOccur((isVertexShader()) ? 1 : 2)) {
    this->shaderHandle = 0;
    return;
  }

  glGetObjectParameterivARB(this->shaderHandle,GL_OBJECT_COMPILE_STATUS_ARB,
			    &flag);
  SoGLSLShaderObject::printInfoLog(this->shaderHandle,(isVertexShader())?1:2);

  if (!flag) this->shaderHandle = 0;
}

void SoGLSLShaderObject::unload()
{
  detach();
  if (this->shaderHandle) glDeleteObjectARB(this->shaderHandle);
  this->shaderHandle  = 0;
  this->programHandle = 0;
}

SoGLShaderParameter* SoGLSLShaderObject::
getParameter(int, const char* name, SoGLShader::ValueType type)
{
  return new SoGLSLShaderParameter(this->programHandle, name, type);
}

// *** public SoGLSLShaderObject interface: *********************************

SoGLSLShaderObject::SoGLSLShaderObject()
{
#ifdef _WIN32
# define PADDR(functype,funcname) \
  (funcname = (functype) wglGetProcAddress( #funcname ))

  static SbBool isInitialized = FALSE;

  if (!isInitialized) {
    isInitialized = TRUE;

    PADDR(PFNGLGETACTIVEUNIFORMARBPROC,     glGetActiveUniformARB);
    PADDR(PFNGLCREATEPROGRAMOBJECTARBPROC,  glCreateProgramObjectARB);
    PADDR(PFNGLCREATESHADEROBJECTARBPROC,   glCreateShaderObjectARB);
    PADDR(PFNGLDELETEOBJECTARBPROC,         glDeleteObjectARB);
    PADDR(PFNGLDETACHOBJECTARBPROC,         glDetachObjectARB);
    PADDR(PFNGLATTACHOBJECTARBPROC,         glAttachObjectARB);
    PADDR(PFNGLSHADERSOURCEARBPROC,         glShaderSourceARB);
    PADDR(PFNGLCOMPILESHADERARBPROC,        glCompileShaderARB);
    PADDR(PFNGLLINKPROGRAMARBPROC,          glLinkProgramARB);
    PADDR(PFNGLGETINFOLOGARBPROC,           glGetInfoLogARB);
    PADDR(PFNGLUSEPROGRAMOBJECTARBPROC,     glUseProgramObjectARB);
    PADDR(PFNGLGETOBJECTPARAMETERIVARBPROC, glGetObjectParameterivARB);
    PADDR(PFNGLGETUNIFORMLOCATIONARBPROC,   glGetUniformLocationARB);
    PADDR(PFNGLUNIFORM1FARBPROC,            glUniform1fARB);
    PADDR(PFNGLUNIFORM2FARBPROC,            glUniform2fARB);
    PADDR(PFNGLUNIFORM3FARBPROC,            glUniform3fARB);
    PADDR(PFNGLUNIFORM4FARBPROC,            glUniform4fARB);
  }
# undef PADDR
#endif
  this->programHandle = 0;
  this->shaderHandle  = 0;
}

SoGLSLShaderObject::~SoGLSLShaderObject()
{
  unload();
}

void SoGLSLShaderObject::attach(GLhandleARB programHandle)
{
  if (programHandle <= 0 || this->programHandle == programHandle) return;
  
  detach();
  
  if (this->shaderHandle) {
    this->programHandle = programHandle;
    glAttachObjectARB(this->programHandle, this->shaderHandle);
  }
}

void SoGLSLShaderObject::detach()
{
  if (this->programHandle && this->shaderHandle) {
    glDetachObjectARB(this->programHandle, this->shaderHandle);
  }
}

// --- static methods -------------------------------------------------------

void SoGLSLShaderObject::printInfoLog(GLhandleARB handle, int objType)
{
  GLint length = 0;
  
  glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
  
  if (length > 1) {
    GLcharARB *infoLog = new GLcharARB[length];
    int charsWritten   = 0;
    glGetInfoLogARB(handle, length, &charsWritten, infoLog);
    std::cerr << "GLSL ";
    switch (objType) {
      case 0: std::cerr << "program ";        break;
      case 1: std::cerr << "vertexShader ";   break;
      case 2: std::cerr << "fragmentShader "; break;
      default: ;// do nothing    
    }
    std::cerr << "InfoLog: " << infoLog << std::endl;
    delete [] infoLog;
  }
}

SbBool SoGLSLShaderObject::didOpenGLErrorOccur(int objType)
{
  GLenum glErr;
  SbBool retCode = FALSE;

  glErr = glGetError();
  while (glErr != GL_NO_ERROR) {
    std::cerr << "GLSL ";
    switch (objType) {
      case 0: std::cerr << "program ";        break;
      case 1: std::cerr << "vertexShader ";   break;
      case 2: std::cerr << "fragmentShader "; break;
      default: ; // do nothing    
    }
    std::cerr << "error: " <<  gluErrorString(glErr) << std::endl;
    retCode = TRUE;
    glErr   = glGetError();
  }
  return retCode;
}

/****************************************************************************
 ***                SoGLSLShaderProgram implementation                    ***
 ****************************************************************************/

SoGLSLShaderProgram::SoGLSLShaderProgram()
{
  this->programHandle  = 0;
  this->shouldLink     = FALSE;
  this->isExecutable   = FALSE;
}

SoGLSLShaderProgram::~SoGLSLShaderProgram()
{
}

void SoGLSLShaderProgram::addShaderObject(SoGLSLShaderObject *shaderObject)
{
  if (shaderObject!=NULL) {
    if (indexOfShaderObject(shaderObject) < 0) {
      this->shaderObjects.push_back(shaderObject);
      this->shouldLink = TRUE;
    }
  }
}

void SoGLSLShaderProgram::removeShaderObject(SoGLSLShaderObject *shaderObject)
{
  int idx = indexOfShaderObject(shaderObject);
  if (idx >= 0) {
    std::vector<SoGLSLShaderObject*>::iterator iter;

    iter = this->shaderObjects.begin() + idx;
    this->shaderObjects[idx]->unload(); // detach();
    this->shaderObjects.erase(iter, iter+1);
    this->shouldLink = TRUE;
  }
}

void SoGLSLShaderProgram::enable()
{
  this->ensureLinking();

  if (this->isExecutable) glUseProgramObjectARB(this->programHandle);
}

void SoGLSLShaderProgram::disable()
{
  if (this->isExecutable) glUseProgramObjectARB(0);
}

void SoGLSLShaderProgram::postShouldLink()
{
  this->shouldLink = TRUE;
}

#if defined(SOURCE_HINT)
SbString SoGLSLShaderProgram::getSourceHint() const
{
  SbString result;  
  for (int i=0; i<this->shaderObjects.size(); i++) {
    SoGLSLShaderObject *shader = this->shaderObjects[i];
    if (shader && shader->isActive()) {      
      SbString str = shader->sourceHint;
      if (str.getLength() > 0) str += " ";
      result += str;
    }
  }
  return result;
}
#endif

void SoGLSLShaderProgram::ensureLinking()
{
  if (!this->shouldLink) return;

  this->isExecutable = FALSE;
  this->ensureProgramHandle();
  
  int cnt = this->shaderObjects.size();

  if (cnt > 0) {
    GLint didLink;

    for (int i=0; i<cnt; i++) 
      this->shaderObjects[i]->attach(this->programHandle);

    glLinkProgramARB(this->programHandle);

    if (SoGLSLShaderObject::didOpenGLErrorOccur(0)) {
      SoGLSLShaderObject::printInfoLog(this->programHandle, 0);
    }
    glGetObjectParameterivARB(this->programHandle,
			      GL_OBJECT_LINK_STATUS_ARB,&didLink);

    this->isExecutable = didLink;
    this->shouldLink   = FALSE;
  }
}

int SoGLSLShaderProgram::indexOfShaderObject(SoGLSLShaderObject *shaderObject)
{
  if (shaderObject == NULL) return -1;
  
  int cnt = this->shaderObjects.size();
  for (int i=0; i<cnt; i++) {
    if (shaderObject == this->shaderObjects[i]) return i;
  }
  return -1;
}

void SoGLSLShaderProgram::ensureProgramHandle()
{
  if (this->programHandle == 0) 
    this->programHandle = glCreateProgramObjectARB();
}

#endif /* SO_GLSL_SHADER_SUPPORT */
