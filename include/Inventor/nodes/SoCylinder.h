/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SOCYLINDER_H
#define COIN_SOCYLINDER_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBitMask.h>


class SoCylinder : public SoShape {
  typedef SoShape inherited;

  SO_NODE_HEADER(SoCylinder);

public:
  static void initClass(void);
  SoCylinder(void);

  enum Part {
    SIDES =  0x1,
    TOP =    0x2,
    BOTTOM = 0x4,
    ALL =    SIDES|TOP|BOTTOM
  };

  SoSFFloat radius;
  SoSFFloat height;
  SoSFBitMask parts;

  void addPart(SoCylinder::Part part);
  void removePart(SoCylinder::Part part);
  SbBool hasPart(SoCylinder::Part part) const;

  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual void GLRender(SoGLRenderAction * action);
  virtual SbBool willSetShapeHints(void) const;
  virtual SbBool willSetShadeModel(void) const;

protected:
  virtual ~SoCylinder();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
};

#endif // !COIN_SOCYLINDER_H
