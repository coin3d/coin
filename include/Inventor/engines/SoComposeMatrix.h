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

#ifndef COIN_SOCOMPOSEMATRIX_H
#define COIN_SOCOMPOSEMATRIX_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFRotation.h>


class COIN_DLL_API SoComposeMatrix : public SoEngine {
  typedef SoEngine inherited;

  SO_COMPOSE__HEADER(SoComposeMatrix);

public:
  SoMFVec3f translation;
  SoMFRotation rotation;
  SoMFVec3f scaleFactor;
  SoMFRotation scaleOrientation;
  SoMFVec3f center;

  SoEngineOutput matrix; // SoMFMatrix
};

#endif // !COIN_SOCOMPOSEMATRIX_H
