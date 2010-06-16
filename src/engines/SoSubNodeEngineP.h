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
#error this is a private header file
#endif // !COIN_INTERNAL

#include <Inventor/engines/SoSubNodeEngine.h>

#include "tidbitsp.h"

// Be aware that any changes to the SO_ENGINE_INTERNAL_CONSTRUCTOR
// macro should be matched by similar changes to the constructor in
// the SO_INTERPOLATE_SOURCE macro (which have to use
// SO_ENGINE_CONSTRUCTOR because it is "public").

// Update 2006-08-11, pederb:
// The INIT_CLASS macros will now set the node type to VRML97. All
// node-engines in Coin are VRML97 nodes. If we add node engines that
// are not VRML97 nodes we need to add some new macros.


#define SO_NODEENGINE_INTERNAL_CONSTRUCTOR(_class_) \
  do { \
    SO_NODEENGINE_CONSTRUCTOR(_class_); \
    /* Restore value of isBuiltIn flag (which is set to FALSE */ \
    /* in the SO_ENGINE_CONSTRUCTOR() macro). */ \
    this->isBuiltIn = TRUE; \
  } while (0)

#define SO_NODEENGINE_INTERNAL_INIT_CLASS(_class_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_COMMON_NODEENGINE_INIT_CODE(_class_, &classname[2], &_class_::createInstance, inherited); \
    SoNode::setCompatibilityTypes(_class_::getClassTypeId(), SO_VRML97_NODE_TYPE); \
    coin_atexit((coin_atexit_f*)_class_::atexit_cleanupnodeengine, CC_ATEXIT_NORMAL); \
    coin_atexit((coin_atexit_f*)_class_::atexit_cleanup, CC_ATEXIT_NORMAL); \
  } while (0)


#define SO_NODEENGINE_INTERNAL_INIT_ABSTRACT_CLASS(_class_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_COMMON_NODEENGINE_INIT_CODE(_class_, &classname[2], NULL, inherited); \
    SoNode::setCompatibilityTypes(_class_::getClassTypeId(), SO_VRML97_NODE_TYPE); \
    coin_atexit((coin_atexit_f*)_class_::atexit_cleanupnodeengine, CC_ATEXIT_NORMAL); \
    coin_atexit((coin_atexit_f*)_class_::atexit_cleanup, CC_ATEXIT_NORMAL); \
  } while (0)


#endif // COIN_SOSUBNODEENGINEP_H
