/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOTEXT3_H
#define COIN_SOTEXT3_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/lists/SbList.h>

class SoSensor;
class SoFieldSensor;
class SoGlyph;

class COIN_DLL_API SoText3 : public SoShape {
  typedef SoShape inherited;

  SO_NODE_HEADER(SoText3);

public:
  static void initClass(void);
  SoText3(void);

  enum Part {
    FRONT = 1,
    SIDES = 2,
    BACK =  4,
    ALL = FRONT|BACK|SIDES
  };

  enum Justification {
    LEFT = 1,
    RIGHT,
    CENTER
  };

  SoMFString string;
  SoSFFloat spacing;
  SoSFEnum justification;
  SoSFBitMask parts;

  SbBox3f getCharacterBounds(SoState * state, int stringindex, int charindex);

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoText3();

  virtual void generatePrimitives(SoAction *);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
  virtual SoDetail * createTriangleDetail(SoRayPickAction * action,
                                         const SoPrimitiveVertex * v1,
                                         const SoPrimitiveVertex * v2,
                                         const SoPrimitiveVertex * v3,
                                         SoPickedPoint * pp);

  virtual void notify(SoNotList *list);
  
private:
  class SoText3P * pimpl;
  friend class SoText3P;
  void render(SoState * state, unsigned int part);
  void generate(SoAction * action, unsigned int part);
};

#endif // !COIN_SOTEXT3_H
