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

#ifndef COIN_SNPRINTF_H
#define COIN_SNPRINTF_H

#ifdef __cplusplus
extern "C" {
#endif


#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#if !HAVE_SNPRINTF
#if HAVE__SNPRINTF
/* This is how it is defined in MSVC++ 5.0. */
#define snprintf _snprintf
#else /* !HAVE__SNPRINTF */
extern int snprintf(char * target, size_t n, const char * formatstr, ...);
#endif /* !HAVE__SNPRINTF */
#endif /* !HAVE_SNPRINTF */

#if !HAVE_VSNPRINTF
#if HAVE__VSNPRINTF
/* This is how it is defined in MSVC++ 5.0. */
#define vsnprintf _vsnprintf
#else /* !HAVE__VSNPRINTF */
extern int vsnprintf(char * target, size_t n, const char * formatstr, va_list args);
#endif /* !HAVE__VSNPRINTF */
#endif /* !HAVE_VSNPRINTF */

#ifdef __cplusplus
}
#endif

#endif /* COIN_SNPRINTF_H */
