#ifndef COIN_SOSUBEVENT_H
#define COIN_SOSUBEVENT_H

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

#include <Inventor/C/tidbits.h>

// *************************************************************************

#define SO_EVENT_HEADER() \
private: \
  static SoType classTypeId; \
  static void cleanupClass(void) { classTypeId STATIC_SOTYPE_INIT; }; \
  static void * createInstance(void); \
public: \
  static SoType getClassTypeId(void); \
  virtual SoType getTypeId(void) const

#define SO_EVENT_ABSTRACT_HEADER() \
private: \
  static SoType classTypeId; \
  static void cleanupClass(void) { classTypeId STATIC_SOTYPE_INIT; }; \
public: \
  static SoType getClassTypeId(void); \
  virtual SoType getTypeId(void) const

// *************************************************************************

#define SO_EVENT_ABSTRACT_SOURCE(_class_) \
SoType _class_::getClassTypeId(void) { return _class_::classTypeId; } \
SoType _class_::getTypeId(void) const { return _class_::classTypeId; } \
SoType _class_::classTypeId STATIC_SOTYPE_INIT

#define SO_EVENT_SOURCE(_class_) \
SoType _class_::getClassTypeId(void) { return _class_::classTypeId; } \
SoType _class_::getTypeId(void) const { return _class_::classTypeId; } \
void * _class_::createInstance(void) { return static_cast<void *>(new _class_); } \
SoType _class_::classTypeId STATIC_SOTYPE_INIT

// *************************************************************************

#define SO_EVENT_INIT_ABSTRACT_CLASS(_class_, _parentclass_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass get initialized before subclass. */ \
    assert(_parentclass_::getClassTypeId() != SoType::badType()); \
    \
    _class_::classTypeId = \
      SoType::createType(_parentclass_::getClassTypeId(), SO__QUOTE(_class_)); \
    /* FIXME: internal code should not use this function, but use the coin_atexit() function */ \
    /* with priority set to CC_ATEXIT_NORMAL. As it is now, the clean-up functions for */ \
    /* these classes will always be run before all other Coin at-exit clean-ups. 20070126 mortene */ \
    cc_coin_atexit(reinterpret_cast<coin_atexit_f *>(cleanupClass)); \
  } while (0)

#define SO_EVENT_INIT_CLASS(_class_, _parentclass_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass get initialized before subclass. */ \
    assert(_parentclass_::getClassTypeId() != SoType::badType()); \
    \
    _class_::classTypeId = \
      SoType::createType(_parentclass_::getClassTypeId(), SO__QUOTE(_class_), _class_::createInstance); \
    /* FIXME: internal code should not use this function, but use the coin_atexit() function */ \
    /* with priority set to CC_ATEXIT_NORMAL. As it is now, the clean-up functions for */ \
    /* these classes will always be run before all other Coin at-exit clean-ups. 20070126 mortene */ \
    cc_coin_atexit(reinterpret_cast<coin_atexit_f *>(cleanupClass)); \
  } while (0)

// *************************************************************************

#endif // !COIN_SOSUBEVENT_H
