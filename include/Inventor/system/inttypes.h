/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_INTTYPES_H
#define COIN_INTTYPES_H

/* ALERT: This is also a C header file, so don't use C++ comments!  larsa */

#include <sys/types.h>

/*
  The following types need to be defined:
    int16_t, uint16_t
    int32_t, uint32_t
*/

#if defined(__GLIBC__) && !defined(__BEOS__)
  typedef u_int32_t uint32_t;
  typedef u_int16_t uint16_t;
#else /* ! __GLIBC__ */

  /* FIXME: this doesn't look very portable - 990610 larsa */
  typedef unsigned short uint16_t;
  typedef unsigned int uint32_t;
#if ! defined(linux) || ! defined(__BIT_TYPES_DEFINED__)
  typedef short int16_t;
  typedef int int32_t;
#endif /* ! defined(linux) || ! defined(__BIT_TYPES_DEFINED__) */

#endif /* ! __GLIBC__ */


#endif /* ! COIN_INTTYPES_H */
