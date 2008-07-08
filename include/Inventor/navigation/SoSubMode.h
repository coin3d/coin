#ifndef COIN_SOSUBMODE_H
#define COIN_SOSUBMODE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

// IMPORTANT NOTE: This class is part of a development DEAD END in
// Coin that WILL BE DEPRECATED and removed in a future version.
// It is only included because of 3rd party dependencies created towards
// the development branch of Coin before the Coin 3.0 release.
// Use ScXML-based navigation instead.  It's the way of the future.

#include <assert.h>
#include <string.h>

#include <Inventor/SoType.h>
#include <Inventor/C/tidbits.h>

#define SO_NAVIGATION_MODE_HEADER(classname) \
public: \
  static SoType classTypeId; \
  static SoType getClassTypeId(void); \
  static void cleanClass(void); \
  virtual SoType getTypeId(void) const

#define SO_NAVIGATION_MODE_SOURCE(classname) \
SoType classname::getTypeId(void) const { \
  return classname::classTypeId; \
} \
SoType classname::getClassTypeId(void) { \
  return classname::classTypeId; \
} \
void classname::cleanClass(void) {\
  SoType::removeType(classname::classTypeId.getName()); \
  classname::classTypeId STATIC_SOTYPE_INIT; \
} \
SoType classname::classTypeId = SoType::badType()

#define SO_NAVIGATION_MODE_INIT_CLASS(classname, parentname) \
  do { \
    assert(classname::classTypeId == SoType::badType() && "don't init() twice!"); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(strcmp(SO__QUOTE(parentname), "inherited")); \
    SoType parentType(SoType::fromName(SO__QUOTE(parentname))); \
    assert(parentType != SoType::badType() && "you forgot init() on parentclass!"); \
 \
    /* Set up entry in the type system. */ \
    classname::classTypeId = \
      SoType::createType(parentType, \
                         SO__QUOTE(classname), \
                         NULL /* createfunc */, \
                         0 /* data */); \
    cc_coin_atexit_static_internal((coin_atexit_f*) classname::cleanClass); \
  } while (0)

#endif // !COIN_SOSUBMODE_H
