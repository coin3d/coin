#ifndef COIN_COINSTATICOBJECTINDLL_H
#define COIN_COINSTATICOBJECTINDLL_H

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

// See .cpp-file for documentation.

// *************************************************************************

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

// *************************************************************************

#include <Inventor/SbString.h>

// *************************************************************************

class CoinStaticObjectInDLL {
public:
  CoinStaticObjectInDLL(void);
  ~CoinStaticObjectInDLL();

  static void init(void);

private:
  static SbBool activateMutex(void);
  static void deactivateMutex(void);
  static SbString mutexName(void);
  static void * mutexhandle;
  static CoinStaticObjectInDLL * singleton;
};

// *************************************************************************

#endif // !COIN_COINSTATICOBJECTINDLL_H
