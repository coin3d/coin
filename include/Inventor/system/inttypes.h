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

#ifndef COIN_INTTYPES_H
#define COIN_INTTYPES_H

#include <sys/types.h>

/*
  The following types need to be defined:
    int16_t, uint16_t
    int32_t, uint32_t
*/

#if defined(__GLIBC__) && !defined(__BEOS__)
  typedef u_int32_t uint32_t;
  typedef u_int16_t uint16_t;
#else // ! __GLIBC__

  // FIXME: this doesn't look very portable - 990610 larsa
  typedef unsigned short uint16_t;
  typedef unsigned int uint32_t;
#if ! defined(linux) || ! defined(__BIT_TYPES_DEFINED__)
  typedef short int16_t;
  typedef int int32_t;
#endif // ! defined(linux) || ! defined(__BIT_TYPES_DEFINED__)

#endif // ! __GLIBC__


#endif // ! COIN_INTTYPES_H
