#ifndef COIN_SOCONTEXTHANDLER_H
#define COIN_SOCONTEXTHANDLER_H

#include <Inventor/SbBasic.h>

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

#include <Inventor/SbBasic.h>

class COIN_DLL_API SoContextHandler {
public:
  static void destructingContext(uint32_t contextid);

  typedef void ContextDestructionCB(uint32_t contextid, void * userdata);
  static void addContextDestructionCallback(ContextDestructionCB * func, void * closure);
  static void removeContextDestructionCallback(ContextDestructionCB * func, void * closure);
};

#endif // !COIN_SOCONTEXTHANDLER_H
