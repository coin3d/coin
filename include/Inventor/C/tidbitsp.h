/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifndef COIN_TIDBITSP_H
#define COIN_TIDBITSP_H

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

#include <stdio.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/basic.h>
#include <Inventor/C/base/string.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* to get proper auto-indentation in emacs */
}
#endif /* emacs indentation */

/* ********************************************************************** */

void coin_init_tidbits(void);

/* ********************************************************************** */

FILE * coin_get_stdin(void);
FILE * coin_get_stdout(void);
FILE * coin_get_stderr(void);

/* ********************************************************************** */

#define coin_atexit(func, priority) \
        coin_atexit_func(SO__QUOTE(func), func, priority)

void coin_atexit_func(const char * name, coin_atexit_f * fp, int32_t priority);
void coin_atexit_cleanup(void);
SbBool coin_is_exiting(void);

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

/*
  Functions to output ascii85 encoded data. Used for instance for Postscript
  image rendering.
*/
void coin_output_ascii85(FILE * fp,
                         const unsigned char val,
                         unsigned char * tuple,
                         unsigned char * linebuf,
                         int * tuplecnt, int * linecnt,
                         const int rowlen,
                         const SbBool flush);

void coin_flush_ascii85(FILE * fp,
                        unsigned char * tuple,
                        unsigned char * linebuf,
                        int * tuplecnt, int * linecnt,
                        const int rowlen);

/* ********************************************************************** */

/*
  Parse version string of type <major>.<minor>.<patch>. <minor> or
  <patch> might not be in the string. It's possible to supply NULL for
  minor and/or patch if you're not interested in minor and/or patch.
*/
SbBool coin_parse_versionstring(const char * versionstr,
                                int * major,
                                int * minor,
                                int * patch);

/* ********************************************************************** */

SbBool coin_getcwd(cc_string * str);

/* ********************************************************************** */

int coin_isinf(double value);
int coin_isnan(double value);
int coin_finite(double value);

/* ********************************************************************** */

unsigned long coin_geq_prime_number(unsigned long num);

/* ********************************************************************** */

enum CoinOSType { 
  COIN_UNIX, 
  COIN_OS_X, 
  COIN_MSWINDOWS 
};

int coin_runtime_os(void);

/* ********************************************************************** */

int coin_debug_extra(void);
int coin_debug_normalize(void);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/* ********************************************************************** */

#endif /* !COIN_TIDBITS_H */
