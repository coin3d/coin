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

#ifndef COIN_DEFS_H
#define COIN_DEFS_H

// This file contains definitions which should _only_ be used during
// library build. It is not installed for use by the application
// programmer.
#ifndef COIN_INTERNAL
#error Only for internal use during library build.
#endif // !COIN_INTERNAL

#include <config.h> // for HAVE_* defines


#ifdef __FILE__
#define COIN_STUB_FILE __FILE__
#else
#define COIN_STUB_FILE ((const char *)0L)
#endif

#ifdef __LINE__
#define COIN_STUB_LINE __LINE__
#else
#define COIN_STUB_LINE 0
#endif

#if HAVE_VAR___PRETTY_FUNCTION__
#define COIN_STUB_FUNC __PRETTY_FUNCTION__
#elif HAVE_VAR___FUNCTION__
#define COIN_STUB_FUNC __FUNCTION__
#else
#define COIN_STUB_FUNC ((const char *)0L)
#endif

// COIN_STUB(): this is the method which prints out stub
// information. Used where there is functionality missing.

#if COIN_DEBUG

#include <stdio.h> // fprintf()

#define COIN_STUB() \
  do { \
    (void)fprintf(stderr, "STUB: functionality not yet completed"); \
    if (COIN_STUB_FILE) { \
      (void)fprintf(stderr, " at %s", COIN_STUB_FILE); \
      if (COIN_STUB_LINE > 0) (void)fprintf(stderr, ":line %u", COIN_STUB_LINE); \
      if (COIN_STUB_FUNC) (void)fprintf(stderr, ":[%s]", COIN_STUB_FUNC); \
    } \
    (void)fprintf(stderr, "\n"); \
  } while (0)

#else // !COIN_DEBUG

#define COIN_STUB()  do { } while (0)

#endif // !COIN_DEBUG


#endif // !COIN_DEFS_H
