/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_DEFS_H
#define COIN_DEFS_H

/*
  This file contains definitions which should _only_ be used during
  library build. It is not installed for use by the application
  programmer.
*/

#ifndef COIN_INTERNAL
#error Only for internal use during library build.
#endif /* !COIN_INTERNAL */

#ifdef HAVE_CONFIG_H
#include <config.h> /* for HAVE_* defines */
#endif /* HAVE_CONFIG_H */


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

#if defined(HAVE_VAR___PRETTY_FUNCTION__)
#define COIN_STUB_FUNC __PRETTY_FUNCTION__
#elif defined(HAVE_VAR___FUNCTION__)
#define COIN_STUB_FUNC __FUNCTION__
#elif defined(HAVE_VAR___func__)
#define COIN_STUB_FUNC __func__
#else
#define COIN_STUB_FUNC ((const char *)0L)
#endif

/*
  COIN_STUB(): this is the method which prints out stub
  information. Used where there is functionality missing.

  COIN_OBSOLETED: this is the method which prints out obsoleted
  information. Used where there is an obsoleted or unsupported
  function. Typically a function that we feel should have been private
  in Open Inventor.
*/

#if COIN_DEBUG

#include <Inventor/errors/SoDebugError.h>

#define COIN_STUB() \
  do { \
    SbString s; \
    s.sprintf("%s:%u:%s", \
              COIN_STUB_FILE ? COIN_STUB_FILE : "<>", \
              COIN_STUB_LINE, \
              COIN_STUB_FUNC ? COIN_STUB_FUNC : "<>"); \
    SoDebugError::postWarning(s.getString(), \
                              "STUB: functionality not yet completed"); \
  } while (0)

#define COIN_STUB_ONCE() \
  do { \
    static int first = 1; \
    if (first) { \
      SbString s; \
      s.sprintf("%s:%u:%s", \
                COIN_STUB_FILE ? COIN_STUB_FILE : "<>", \
                COIN_STUB_LINE, \
                COIN_STUB_FUNC ? COIN_STUB_FUNC : "<>"); \
      SoDebugError::postWarning(s.getString(), \
                                "STUB: functionality not yet completed"); \
      first = 0; \
    } \
  } while (0)

#define COIN_OBSOLETED() \
  do { \
    SbString s; \
    s.sprintf("%s:%u:%s", \
              COIN_STUB_FILE ? COIN_STUB_FILE : "<>", \
              COIN_STUB_LINE, \
              COIN_STUB_FUNC ? COIN_STUB_FUNC : "<>"); \
    SoDebugError::post(s.getString(), \
                       "OBSOLETED: functionality not supported (any more)"); \
  } while (0)

#else /* !COIN_DEBUG */

#define COIN_STUB()       do { } while (0)
#define COIN_OBSOLETED()  do { } while (0)

#endif /* !COIN_DEBUG */


#endif /* !COIN_DEFS_H */
