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

#ifndef COIN_DEFS_H
#define COIN_DEFS_H

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

#ifdef __PRETTY_FUNCTION__
#define COIN_STUB_FUNC __PRETTY_FUNCTION__
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
