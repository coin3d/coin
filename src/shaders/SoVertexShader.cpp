#include <Inventor/nodes/SoVertexShader.h>

SO_NODE_SOURCE(SoVertexShader);

void SoVertexShader::initClass()
{
  SO_NODE_INIT_CLASS(SoVertexShader, SoShaderObject, "SoShaderObject");
}

SoVertexShader::SoVertexShader()
{
  SO_NODE_CONSTRUCTOR(SoVertexShader);
}

SoVertexShader::~SoVertexShader()
{
  
}

SbBool SoVertexShader::isVertexShader() const
{
  return TRUE;
}

SbBool SoVertexShader::isSupported(SourceType sourceType)
{
  // FIXME: Martin
#if !defined(SO_ARB_SHADER_SUPPORT)
  if (sourceType == ARB_PROGRAM) return FALSE;
#endif
#if !defined(SO_CG_SHADER_SUPPORT)
  if (sourceType == CG_PROGRAM) return FALSE;
#endif
#if !defined(SO_GLSL_SHADER_SUPPORT)
  if (sourceType == GLSL_PROGRAM) return FALSE;
#endif
  return TRUE;
}

