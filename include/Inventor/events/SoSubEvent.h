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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSUBEVENT_H
#define COIN_SOSUBEVENT_H

#include <Inventor/events/SoEvent.h>


#define SO_EVENT_HEADER() \
private: \
  static SoType classTypeId; \
public: \
  static SoType getClassTypeId(void); \
  virtual SoType getTypeId(void) const


#define SO_EVENT_SOURCE(_class_) \
SoType _class_::getClassTypeId(void) { return _class_::classTypeId; } \
SoType _class_::getTypeId(void) const { return _class_::classTypeId; } \
/* Don't set value explicitly to SoType::badType(), to avoid a bug in */ \
/* Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()). */ \
SoType _class_::classTypeId


#define SO_EVENT_INIT_CLASS(_class_, _parentclass_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass get initialized before subclass. */ \
    assert(_parentclass_::getClassTypeId() != SoType::badType()); \
    \
    _class_::classTypeId = \
      SoType::createType(_parentclass_::getClassTypeId(), SO__QUOTE(_class_)); \
  } while (0)


#endif // !COIN_SOSUBEVENT_H
