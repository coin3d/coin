#ifndef COIN_SOSHADERPROGRAM_H
#define COIN_SOSHADERPROGRAM_H

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
#include <Inventor/fields/SoMFNode.h>

class SoState;
class SoGLRenderAction;

// *************************************************************************

class COIN_DLL_API SoShaderProgram : public SoGroup {
  typedef SoGroup inherited;
  SO_NODE_HEADER(SoShaderProgram);
  
public:
  SoMFNode shaderObject;
  SoShaderProgram(void);
  
SoEXTENDER public:
  virtual void GLRender(SoGLRenderAction * action);
 
SoINTERNAL public:
  static void initClass();

  void updateStateMatrixParameters(SoState * state);
  SbBool containStateMatrixParameters(void) const;

protected:  
  virtual ~SoShaderProgram();

private:
  class SoShaderProgramP * pimpl;
};

#endif /* ! COIN_SOSHADERPROGRAM_H */
