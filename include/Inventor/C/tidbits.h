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

COIN_DLL_API const char * coin_getenv(const char * name);
COIN_DLL_API SbBool coin_setenv(const char * name, const char * value, int overwrite);
COIN_DLL_API void coin_unsetenv(const char * name);

COIN_DLL_API int coin_strncasecmp(const char * str1, const char * str2, int len);
  
COIN_DLL_API uint16_t coin_hton_uint16(uint16_t value);
COIN_DLL_API uint16_t coin_ntoh_uint16(uint16_t value);
COIN_DLL_API uint32_t coin_hton_uint32(uint32_t value);
COIN_DLL_API uint32_t coin_ntoh_uint32(uint32_t value);
COIN_DLL_API uint64_t coin_hton_uint64(uint64_t value);
COIN_DLL_API uint64_t coin_ntoh_uint64(uint64_t value);

COIN_DLL_API void coin_hton_float_bytes(float value, char * result); /* expects room for 4 bytes in result*/
COIN_DLL_API float coin_ntoh_float_bytes(const char * value);   /* expects 4 bytes input */

COIN_DLL_API void coin_hton_double_bytes(double value, char * result); /* expects room for 8 bytes in result */
COIN_DLL_API double coin_ntoh_double_bytes(const char * value); /* expects 8 bytes input */

COIN_DLL_API SbBool coin_isascii(const int c);
COIN_DLL_API SbBool coin_isspace(const char c);

COIN_DLL_API SbBool coin_is_power_of_two(uint32_t x);
COIN_DLL_API uint32_t coin_next_power_of_two(uint32_t x);
COIN_DLL_API uint32_t coin_geq_power_of_two(uint32_t x);

COIN_DLL_API void coin_viewvolume_jitter(int numpasses, int curpass, const int * vpsize, float * jitter);

typedef void coin_atexit_f(void);
COIN_DLL_API void cc_coin_atexit(coin_atexit_f * fp);

/* Used internally to clean up static data. Do not use in application code */
COIN_DLL_API void cc_coin_atexit_static_internal(coin_atexit_f * fp);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !COIN_TIDBITS_H */
