#ifndef COIN_SOSUBACTIONP_H
#define COIN_SOSUBACTIONP_H

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

// This file contains macros which are private to the
// implementation. The header file is not installed for the Coin
// development system.

#include <Inventor/actions/SoSubAction.h>
#include "tidbitsp.h" // for coin_atexit()

// Internally, we use this macro instead of SO_ACTION_ADD_METHOD(). It
// should be used from the constructor, and can not be used from
// within initClass() (as the action classes are initialized before
// the node classes).

#define SO_ACTION_ADD_METHOD_INTERNAL(nodeclass, method) \
  do { \
    SO_ACTION_ADD_METHOD(nodeclass, method); \
  } while (0)

#define SO_ACTION_INTERNAL_INIT_CLASS(_classname_, _parentclassname_) \
   do { \
     SO_ACTION_INIT_CLASS(_classname_, _parentclassname_); \
   } while (0)

#endif // !COIN_SOSUBACTIONP_H
