/*** -*- Mode: C++ -*- *****************************************************
 *
 *
 ***************************************************************************/

#ifndef _SO_GL_SHADER_TYPES_H__
#define _SO_GL_SHADER_TYPES_H__

namespace SoGLShader {

  enum ShaderType {
    ARB_SHADER,
    CG_SHADER,
    GLSL_SHADER
  };

  enum ValueType {
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    TEXTURE1D,
    TEXTURE2D,
    TEXTURE3D,
    TEXTURE_CUBE,
    TEXTURE_RECT,
    FLOAT_MATRIX2,
    FLOAT_MATRIX3,
    FLOAT_MATRIX4,
    UNKNOWN_TYPE
  };
}

#endif /* _SO_GL_SHADER_TYPES_H__ */
