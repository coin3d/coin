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

#ifndef COIN_SOCONE_H
#define COIN_SOCONE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoShape.h>


class SoCone : public SoShape {
  typedef SoShape inherited;

  SO_NODE_HEADER(SoCone);

public:
  static void initClass(void);
  SoCone(void);

  enum Part {
    SIDES = 1,
    BOTTOM,
    ALL
  };

  SoSFBitMask parts;
  SoSFFloat bottomRadius;
  SoSFFloat height;

  void addPart(SoCone::Part part);
  void removePart(SoCone::Part part);
  SbBool hasPart(SoCone::Part part) const;

  virtual void GLRender(SoGLRenderAction * action);
  virtual SbBool willSetShapeHints() const;
  virtual SbBool willSetShadeModel() const;

  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoCone();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
};

#endif // !COIN_SOCONE_H
