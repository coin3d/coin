/* *** -*-c++-*- *************************************************************
 *
 * ***************************************************************************/

#ifndef _SO_SHADER_OBJECT_H_
#define _SO_SHADER_OBJECT_H_

#include <Inventor/SbBasic.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFNode.h>

#include "SoShaders.h"

class SoGLShaderObject;
class SoGLShaderProgram;

class SoShaderObject : public SoGroup {
    
  SO_NODE_ABSTRACT_HEADER(SoShaderObject);
  
public:

  enum SourceType {
      ARB_PROGRAM,
      CG_PROGRAM,
      GLSL_PROGRAM,
      FILENAME
  };

  SoSFBool   isActive;
  SoSFEnum   sourceType;
  SoSFString sourceProgram;
  SoMFNode   parameter;

SoINTERNAL public:
  static void initClass();

  void GLRender(SoGLRenderAction *action);

  // Really send the parameters from start to start + num to the shaders.
  void updateParameters(int start, int num);

  // Really send all the parameters to the shaders.
  void updateAllParameters();

  // Update state matrix paramaters
  void updateStateMatrixParameters();

  // Returns TRUE if at least one parameter is of 
  // SoShaderStateMatrixParameter type
  SbBool containStateMatrixParameters() const;

  SoGLShaderObject * getGLShaderObject() const;
  
  virtual SbBool isVertexShader() const = 0;

  SourceType getSourceType() const;
  SbString getSourceProgram() const;

SoINTERNAL public: // additional non TGS API:

  void removeGLShaderFromGLProgram(SoGLShaderProgram* glProgram);

protected:
  SoShaderObject();
  virtual ~SoShaderObject();

private:
  class SoShaderObjectP * pimpl;
};

#endif /*_SO_SHADER_OBJECT_H_*/
