/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOCYLINDER_H
#define COIN_SOCYLINDER_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBitMask.h>


class COIN_DLL_EXPORT SoCylinder : public SoShape {
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

protected:
  virtual ~SoCylinder();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
};

#endif // !COIN_SOCYLINDER_H
