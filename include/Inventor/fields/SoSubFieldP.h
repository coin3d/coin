#ifndef COIN_SOSUBFIELDP_H
#define COIN_SOSUBFIELDP_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

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
