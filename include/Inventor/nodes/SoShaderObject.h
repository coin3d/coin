#ifndef COIN_SOSHADEROBJECT_H
#define COIN_SOSHADEROBJECT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFNode.h>

class SoGLShaderObject;
class SoGLShaderProgram;

// *************************************************************************

class COIN_DLL_API SoShaderObject : public SoGroup {

  SO_NODE_ABSTRACT_HEADER(SoShaderObject);

public:

  enum SourceType {
    ARB_PROGRAM,
    CG_PROGRAM,
    GLSL_PROGRAM,
    FILENAME
  };

  SoSFBool isActive;
  SoSFEnum sourceType;
  SoSFString sourceProgram;
  SoMFNode parameter;

  static void initClass(void);

  void GLRender(SoGLRenderAction * action);

  // Really send the parameters from start to start + num to the shaders. 
  void updateParameters(int start, int num);

  // Really send all the parameters to the shaders. 
  void updateAllParameters(void);

  // Update state matrix paramaters
  void updateStateMatrixParameters(void);

  // Returns TRUE if at least one parameter is of 
  // SoShaderStateMatrixParameter type
  SbBool containStateMatrixParameters(void) const;

  SoGLShaderObject * getGLShaderObject(void) const;

  virtual SbBool isVertexShader(void) const = 0;

  SourceType getSourceType(void) const;
  SbString getSourceProgram(void) const;

  // additional non TGS API:
  void removeGLShaderFromGLProgram(SoGLShaderProgram* glProgram);

protected:
  SoShaderObject(void);
  virtual ~SoShaderObject();

private:
  class SoShaderObjectP * pimpl;
};

#endif /* ! COIN_SOSHADEROBJECT_H*/
