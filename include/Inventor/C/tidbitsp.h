/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion. All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_TIDBITSP_H
#define COIN_TIDBITSP_H

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

#include <stdio.h>
#include <Inventor/C/basic.h>
#include <Inventor/C/base/string.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* to get proper auto-indentation in emacs */
}
#endif /* emacs indentation */

/* ********************************************************************** */

FILE * coin_get_stdin(void);
FILE * coin_get_stdout(void);
FILE * coin_get_stderr(void);

/* ********************************************************************** */

typedef void coin_atexit_f(void);
void coin_atexit(coin_atexit_f *, uint32_t priority);

void coin_atexit_cleanup(void);

/* ********************************************************************** */

/*
  We're using these to ensure portable import and export even when the
  application sets a locale with different thousands separator and
  decimal point than the default "C" locale.

  Use these functions to wrap locale-aware functions where
  necessary:

  \code
  cc_string storedlocale;
  SbBool changed = coin_locale_set_portable(&storedlocale);

  // [code with locale-aware functions]

  if (changed) { coin_locale_reset(&storedlocale); }
  \endcode

  Possibly locale-aware functions includes at least atof(), atoi(),
  atol(), strtol(), strtoul(), strtod(), strtof(), strtold(), and all
  the *printf() functions.
*/

SbBool coin_locale_set_portable(cc_string * storeold);
void coin_locale_reset(cc_string * storedold);

/*
  Portable atof() function, which will not cause any trouble due to
  underlying locale's decimal point setting.
*/
double coin_atof(const char * ptr);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/* ********************************************************************** */

#endif /* !COIN_TIDBITS_H */
