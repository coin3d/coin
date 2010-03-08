#ifndef COIN_SOSUBKITP_H
#define COIN_SOSUBKITP_H

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

// The macro definitions in this file is used internally by Coin
// classes, and mirrors some of the public macros available in
// SoSubKit.h with a few modifications so they are suited for the
// builtin classes.
//
// The macros in this file are not made visible for use by the
// application programmer.


#ifndef COIN_INTERNAL
#error this is a private header file
#endif // !COIN_INTERNAL

#include "nodes/SoSubNodeP.h"

// Note for developers: match changes to this macro with the same
// changes to SoBaseKit::initClass().
#define SO_KIT_INTERNAL_INIT_CLASS(_class_, _fileformats_) \
  do { \
    SO_NODE_INTERNAL_INIT_CLASS(_class_, _fileformats_); \
    _class_::parentcatalogptr = inherited::getClassNodekitCatalogPtr(); \
  } while (0)


#define SO_KIT_INTERNAL_CONSTRUCTOR(_class_) \
  do { \
    SO_NODE_INTERNAL_CONSTRUCTOR(_class_); \
    SoBase::staticDataLock(); \
    if (_class_::classcatalog == NULL) { \
      SoType mytype = SoType::fromName(SO__QUOTE(_class_)); \
      if (_class_::parentcatalogptr) \
        _class_::classcatalog = (*_class_::parentcatalogptr)->clone(mytype); \
      else \
        _class_::classcatalog = new SoNodekitCatalog; \
      cc_coin_atexit_static_internal(reinterpret_cast<coin_atexit_f*>(_class_::atexit_cleanupkit)); \
    } \
    SoBase::staticDataUnlock(); \
  } while (0)

#endif // !COIN_SOSUBKITP_H
