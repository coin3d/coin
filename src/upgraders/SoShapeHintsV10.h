#ifndef COIN_SOSHAPEHINTV10_H
#define COIN_SOSHAPEHINTV10_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoSubNode.h>

class SoShapeHints;

class COIN_DLL_API SoShapeHintsV10 : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoShapeHintsV10);

public:

  // These are the old hints
  enum Hint {
    SURFACE = 0x00, UNORDERED, CONCAVE, SOLID, ORDERED, CONVEX
  };

  // These are the two old fields
  SoSFBitMask hints;
  SoSFFloat creaseAngle;

  static void initClass(void);
  SoShapeHintsV10(void);

  SoShapeHints * createUpgrade(void) const;

protected:
  virtual ~SoShapeHintsV10();
};

#endif // !COIN_SOSHAPEHINTSV10_H
