#ifndef COIN_SOSUBDETAIL_H
#define COIN_SOSUBDETAIL_H

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

#include <Inventor/details/SoDetail.h>
#include <Inventor/C/tidbits.h> // for cc_coin_atexit()
#include <Inventor/SbName.h> // for implicit char* -> SbName cast in
                             // SoType::createType()
#include <assert.h>

// *************************************************************************

#define SO_DETAIL_HEADER(_class_) \
public: \
  virtual SoType getTypeId(void) const; \
  static SoType getClassTypeId(void); \
private: \
  static SoType classTypeId; \
public: \
  static void cleanupClass(void) { _class_::classTypeId STATIC_SOTYPE_INIT; }

// *************************************************************************

#define SO_DETAIL_SOURCE(_class_) \
SoType _class_::classTypeId STATIC_SOTYPE_INIT; \
SoType _class_::getTypeId(void) const { return _class_::classTypeId; } \
SoType _class_::getClassTypeId(void) { return _class_::classTypeId; }

// *************************************************************************

#define SO_DETAIL_INIT_CLASS(_class_, _parentclass_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass get initialized before subclass. */ \
    assert(_parentclass_::getClassTypeId() != SoType::badType()); \
    \
    _class_::classTypeId = \
           SoType::createType(_parentclass_::getClassTypeId(), \
                              SO__QUOTE(_class_)); \
    /* FIXME: internal code should not use this function, but use the coin_atexit() function */ \
    /* with priority set to CC_ATEXIT_NORMAL. As it is now, the clean-up functions for */ \
    /* these classes will always be run before all other Coin at-exit clean-ups. 20070126 mortene */ \
    cc_coin_atexit(reinterpret_cast<coin_atexit_f *>(_class_::cleanupClass)); \
  } while (0)

// *************************************************************************

#endif // !COIN_SOSUBDETAIL_H
