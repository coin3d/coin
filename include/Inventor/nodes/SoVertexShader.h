/* ***-*-c++-*-**************************************************************
 *
 * **************************************************************************/

#ifndef _SO_VERTEX_SHADER_H_
#define _SO_VERTEX_SHADER_H_

#include "SoShaderObject.h"

class UMDSOSHADER_EXPORT SoVertexShader : public SoShaderObject {

  SO_NODE_HEADER(SoVertexShader);

public:
  static void initClass();
  SoVertexShader();
  virtual ~SoVertexShader();

  static SbBool isSupported(SourceType sourceType);

SoINTERNAL public:
  virtual SbBool isVertexShader() const;
};
#endif
