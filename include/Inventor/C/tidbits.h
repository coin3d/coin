/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_TIDBITS_H
#define COIN_TIDBITS_H

#include <Inventor/C/basic.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */

/* ********************************************************************** */

enum CoinEndiannessValues {
  COIN_HOST_IS_UNKNOWNENDIAN = -1,
  COIN_HOST_IS_LITTLEENDIAN = 0,
  COIN_HOST_IS_BIGENDIAN = 1
};

COIN_DLL_API int coin_host_get_endianness(void);

COIN_DLL_API int coin_snprintf(char * dst, unsigned int n, const char * fmtstr, ...);
COIN_DLL_API int coin_vsnprintf(char * dst, unsigned int n, const char * fmtstr, va_list args);

COIN_DLL_API const char * coin_getenv(const char *);
COIN_DLL_API SbBool coin_setenv(const char *, const char *, int);
COIN_DLL_API void coin_unsetenv(const char *);

COIN_DLL_API int coin_strncasecmp(const char *, const char *, int);

/* int coin_host_get_endianness(void); - internal, declared static */

COIN_DLL_API uint16_t coin_hton_uint16(uint16_t value);
COIN_DLL_API uint16_t coin_ntoh_uint16(uint16_t value);
COIN_DLL_API uint32_t coin_hton_uint32(uint32_t value);
COIN_DLL_API uint32_t coin_ntoh_uint32(uint32_t value);
COIN_DLL_API float coin_hton_float(float value);
COIN_DLL_API float coin_ntoh_float(float value);

COIN_DLL_API SbBool coin_isascii(const int c);
COIN_DLL_API SbBool coin_isspace(const char c);

COIN_DLL_API SbBool coin_is_power_of_two(uint32_t x);
COIN_DLL_API uint32_t coin_next_power_of_two(uint32_t x);
COIN_DLL_API uint32_t coin_geq_power_of_two(uint32_t x);

COIN_DLL_API void coin_viewvolume_jitter(int numpasses, int curpass, const int * vpsize, float * jitter);

typedef void coin_atexit_f(void);
COIN_DLL_API void coin_atexit(coin_atexit_f *);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !COIN_TIDBITS_H */
