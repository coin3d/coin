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

#ifndef COIN_SOSUBKITP_H
#define COIN_SOSUBKITP_H

// The macro definitions in this file is used internally by Coin
// classes, and mirrors some of the public macros available in
// SoSubKit.h with a few modifications so they are suited for the
// builtin classes.
//
// The macros in this file are not made visible for use by the
// application programmer.


#ifndef COIN_INTERNAL
#error Only for use during library build.
#endif // !COIN_INTERNAL


#include <Inventor/nodes/SoSubNodeP.h>


// Note for developers: match changes to this macro with the same
// changes to SoBaseKit::initClass().
#define SO_KIT_INTERNAL_INIT_CLASS(_class_) \
  do { \
    SO_NODE_INTERNAL_INIT_CLASS(_class_); \
    _class_::parentcatalogptr = inherited::getClassNodekitCatalogPtr(); \
  } while (0)


#define SO_KIT_INTERNAL_CONSTRUCTOR(_class_) \
  do { \
    SO_NODE_INTERNAL_CONSTRUCTOR(_class_); \
    if (SO_KIT_IS_FIRST_INSTANCE()) { \
      SoType mytype = SoType::fromName(SO__QUOTE(_class_)); \
      if (_class_::parentcatalogptr) \
        _class_::classcatalog = (*_class_::parentcatalogptr)->clone(mytype); \
      else \
        _class_::classcatalog = new SoNodekitCatalog; \
    } \
  } while (0)


#endif // !COIN_SOSUBKITP_H
