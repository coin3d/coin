/* *** -*-c++-*- *************************************************************
 *
 * ***************************************************************************/

#ifndef _SO_FRAGMENT_SHADER_H_
#define _SO_FRAGMENT_SHADER_H_

#include "SoShaderObject.h"

class UMDSOSHADER_EXPORT SoFragmentShader : public SoShaderObject {

  SO_NODE_HEADER(SoFragmentShader);

public:
  static void initClass();
  SoFragmentShader();
  virtual ~SoFragmentShader();
  
  static SbBool isSupported(SourceType sourceType);

SoINTERNAL public:
  virtual SbBool isVertexShader() const ;   // Returns FALSE
};

#endif /* _SO_FRAGMENT_SHADER_ */
