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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOLEVELOFSIMPLIFICATION_H
#define COIN_SOLEVELOFSIMPLIFICATION_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoLOD.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoSFInt32.h>


class COIN_DLL_API SoLevelOfSimplification : public SoLOD {
  typedef SoLOD inherited;

  SO_NODE_HEADER(SoLevelOfSimplification);

public:
  static void initClass(void);
  SoLevelOfSimplification(void);

  SoMFFloat percentages;
  SoSFInt32 numTriangles;

  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoLevelOfSimplification();

  virtual int whichToTraverse(SoAction *);
};

#endif // !COIN_SOLEVELOFSIMPLIFICATION_H
