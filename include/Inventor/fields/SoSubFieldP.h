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

#ifndef COIN_SOSUBFIELDP_H
#define COIN_SOSUBFIELDP_H

// The macro definitions in this file is used internally by Coin
// classes, and mirrors some of the public macros available in
// SoSubField.h with a few modifications so they are suited for the
// builtin classes.
//
// The macros in this file are not made visible for use by the
// application programmer.

#ifndef COIN_INTERNAL
#error Only used during library build.
#endif // !COIN_INTERNAL


#define SO_SFIELD_INTERNAL_INIT_CLASS(_class_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_FIELD_INIT_CLASS(_class_, &classname[2], inherited, &_class_::createInstance); \
  } while (0)


#define SO_MFIELD_INTERNAL_INIT_CLASS(_class_) \
  SO_SFIELD_INTERNAL_INIT_CLASS(_class_)


#endif // !COIN_SOSUBFIELDP_H
