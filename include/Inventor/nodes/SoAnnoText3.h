#ifndef COIN_SOANNOTEXT3_H
#define COIN_SOANNOTEXT3_H

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

#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFNode.h>

// *************************************************************************

class COIN_DLL_API SoAnnoText3 : public SoShape {
  typedef SoShape inherited;

  SO_NODE_HEADER(SoAnnoText3);

public:
  static void initClass(void);
  SoAnnoText3(void);

  enum Justification {
    LEFT = 1, RIGHT, CENTER
  };

  SoMFString string;
  SoSFEnum justification;
  SoSFFloat spacing;
  SoSFNode alternateRep;


  SbBox3f getCharacterBounds(SoState * state, int stringidx, int charidx);

  virtual void notify(SoNotList * l);

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void write(SoWriteAction * action);

protected:
  virtual ~SoAnnoText3();

  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
  virtual void generatePrimitives(SoAction * action);

private:
  class SoAnnoText3P * pimpl;
};

// *************************************************************************

#endif // ! COIN_SOANNOTEXT3_H
