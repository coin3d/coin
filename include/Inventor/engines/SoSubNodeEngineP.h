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

#ifndef COIN_SOSUBNODEENGINEP_H
#define COIN_SOSUBNODEENGINEP_H

// The macro definitions in this file is used internally by Coin
// classes, and mirrors some of the public macros available in
// SoSubEngine.h with a few modifications so they are suited for the
// builtin classes.
//
// The macros in this file are not made visible for use by the
// application programmer.

#ifndef COIN_INTERNAL
#error Only for use during library build.
#endif // !COIN_INTERNAL

#include <Inventor/engines/SoSubNodeEngine.h>

// Be aware that any changes to the SO_ENGINE_INTERNAL_CONSTRUCTOR
// macro should be matched by similar changes to the constructor in
// the SO_INTERPOLATE_SOURCE macro (which have to use
// SO_ENGINE_CONSTRUCTOR because it is "public").

#define SO_NODEENGINE_INTERNAL_CONSTRUCTOR(_class_) \
  do { \
    SO_NODEENGINE_CONSTRUCTOR(_class_); \
    /* Restore value of isBuiltIn flag (which is set to FALSE */ \
    /* in the SO_ENGINE_CONSTRUCTOR() macro). */ \
    this->isBuiltIn = TRUE; \
  } while (0)


#endif // COIN_SOSUBNODEENGINEP_H
