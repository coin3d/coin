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

#ifndef COIN_SOSUBFIELDPRIVATE_H
#define COIN_SOSUBFIELDPRIVATE_H

#ifndef COIN_INTERNAL
#error Only used during library build.
#endif // !COIN_INTERNAL


// This file contains macros used when building the field classes
// which should not be visible to the application programmer.


#define SO_SFIELD_INTERNAL_INIT_CLASS(_class_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_FIELD_INIT_CLASS(_class_, &classname[2], inherited, &_class_::createInstance); \
  } while (0)


#define SO_MFIELD_INTERNAL_INIT_CLASS(_class_) \
  SO_SFIELD_INTERNAL_INIT_CLASS(_class_)


#endif // !COIN_SOSUBFIELDPRIVATE_H
