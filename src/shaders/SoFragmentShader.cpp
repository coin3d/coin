#include <Inventor/nodes/SoFragmentShader.h>

SO_NODE_SOURCE(SoFragmentShader);

void SoFragmentShader::initClass()
{
  SO_NODE_INIT_CLASS(SoFragmentShader, SoShaderObject, "SoShaderObject");
}

SoFragmentShader::SoFragmentShader()
{
  SO_NODE_CONSTRUCTOR(SoFragmentShader);
}

SoFragmentShader::~SoFragmentShader()
{  
}

SbBool SoFragmentShader::isVertexShader() const
{
  return FALSE;
}

SbBool SoFragmentShader::isSupported(SourceType sourceType)
{
  // FIXME: 20041001 martin
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
