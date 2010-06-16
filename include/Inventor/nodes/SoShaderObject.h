#ifndef COIN_SOSHADEROBJECT_H
#define COIN_SOSHADEROBJECT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/C/glue/gl.h>

class SoGLShaderObject;
class SoGLShaderProgram;
class SoState;

// *************************************************************************

class COIN_DLL_API SoShaderObject : public SoNode {
  typedef SoNode inherited;
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
  // FIXME: this field is an SoMFUniformShaderParameter in TGS
  // Inventor. We should also implement that field. 20050125 mortene.
  SoMFNode parameter;

  static void initClass(void);

  virtual void GLRender(SoGLRenderAction * action);
  virtual void search(SoSearchAction * action);

  void updateParameters(SoState * state);

  SourceType getSourceType(void) const;
  SbString getSourceProgram(void) const;

protected:
  SoShaderObject(void);
  virtual ~SoShaderObject();
  virtual SbBool readInstance(SoInput * in, unsigned short flags);

private:
  class SoShaderObjectP * pimpl;
};

#endif /* ! COIN_SOSHADEROBJECT_H*/
