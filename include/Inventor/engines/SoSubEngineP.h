/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SOSUBENGINEP_H
#define COIN_SOSUBENGINEP_H

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

#include <Inventor/engines/SoSubEngine.h>

// Be aware that any changes to the SO_ENGINE_INTERNAL_CONSTRUCTOR
// macro should be matched by similar changes to the constructor in
// the SO_INTERPOLATE_SOURCE macro (which have to use
// SO_ENGINE_CONSTRUCTOR because it is "public").

#define SO_ENGINE_INTERNAL_CONSTRUCTOR(_class_) \
  do { \
    SO_ENGINE_CONSTRUCTOR(_class_); \
    /* Restore value of isBuiltIn flag (which is set to FALSE */ \
    /* in the SO_ENGINE_CONSTRUCTOR() macro). */ \
    this->isBuiltIn = TRUE; \
  } while (0)


#define SO_ENGINE_INTERNAL_INIT_CLASS(_class_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_COMMON_ENGINE_INIT_CODE(_class_, &classname[2], &_class_::createInstance, inherited); \
  } while (0)


#define SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(_class_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_COMMON_ENGINE_INIT_CODE(_class_, &classname[2], NULL, inherited); \
  } while (0)


#define SO_INTERPOLATE_INTERNAL_INIT_CLASS(_class_) \
 \
void \
_class_::initClass(void) \
{ \
  SO_ENGINE_INTERNAL_INIT_CLASS(_class_); \
}


#endif // !COIN_SOSUBENGINEP_H
