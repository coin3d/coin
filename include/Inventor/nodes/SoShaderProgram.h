/* *** -*-c++-*- ************************************************************
 *
 * **************************************************************************/

#ifndef _SO_SHADER_PROGRAM_H_
#define _SO_SHADER_PROGRAM_H_

#include <Inventor/SbBasic.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFNode.h>
#include "SoShaders.h"

class SoState;
class SoGLRenderAction;

class UMDSOSHADER_EXPORT SoShaderProgram : public SoGroup {

typedef SoGroup inherited;
    
  SO_NODE_HEADER(SoShaderProgram);
  
public:
  SoMFNode shaderObject;
  SoShaderProgram();
  
SoEXTENDER public:
  virtual void GLRender(SoGLRenderAction *action);
 
SoINTERNAL public:
  static void initClass();

  void updateStateMatrixParameters(SoState *state);
  SbBool containStateMatrixParameters() const;

protected:  
  virtual ~SoShaderProgram();

private:
  class SoShaderProgramP * pimpl;
};

#endif /*_SO_SHADER_PROGRAM_H_*/
