/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOCOMPOSEVEC4F_H__
#define __SOCOMPOSEVEC4F_H__

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFVec4f.h>


class SoComposeVec4f : public SoEngine {
  SO_COMPOSE__HEADER(SoComposeVec4f);

public:
  SoMFFloat x;
  SoMFFloat y;
  SoMFFloat z;
  SoMFFloat w;

  SoEngineOutput vector; // SoMFVec4f
};

#endif // !__SOCOMPOSEVEC4F_H__
