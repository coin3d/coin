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

#ifndef COIN_SOSUBACTIONP_H
#define COIN_SOSUBACTIONP_H

// This file contains macros which are private to the
// implementation. The header file is not installed for the Coin
// development system.

#include <Inventor/actions/SoSubAction.h>


// Internally, we use this macro instead of SO_ACTION_ADD_METHOD(). It
// should be used from the constructor, and can not be used from
// within initClass() (as the action classes are initialized before
// the node classes).

#define SO_ACTION_ADD_METHOD_INTERNAL(nodeclass, method) \
  do { \
    SO_ACTION_ADD_METHOD(nodeclass, method); \
  } while (0)



#endif // !COIN_SOSUBACTIONP_H
