#ifndef COIN_MATH_UNDEFS_H
#define COIN_MATH_UNDEFS_H

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

#ifndef COIN_INTERNAL
#error This header is only included while building Coin, and this is not while building Coin it seems.
#endif // !COIN_INTERNAL

/* *********************************************************************** */

/*
  The float-version of the math functions below did not come about
  until C99, so we don't want to use them internally, for portability
  reasons.

  This block was originally part of Inventor/C/basic.h, but since the
  #undef's were mangled by the config.status process, it did not really
  work as intended.  20070518 larsa
*/

#undef cosf
#define cosf(x) NO_SINGLEPREC /* whatever that'll give us a compile error... */
#undef sinf
#define sinf(x) NO_SINGLEPREC
#undef tanf
#define tanf(x) NO_SINGLEPREC
#undef powf
#define powf(x) NO_SINGLEPREC
#undef sqrtf
#define sqrtf(x) NO_SINGLEPREC
#undef acosf
#define acosf(x) NO_SINGLEPREC
#undef asinf
#define asinf(x) NO_SINGLEPREC
#undef atanf
#define atanf(x) NO_SINGLEPREC
#undef atan2f
#define atan2f(x) NO_SINGLEPREC

/* *********************************************************************** */

#endif // !COIN_MATH_UNDEFS_H
