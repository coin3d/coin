#ifndef CC_STRING_H
#define CC_STRING_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

enum cc_string_constants {
  CC_STRING_MIN_SIZE = 128 - sizeof(char *) + sizeof(int)
};

struct cc_string {
  char * pointer;
  int bufsize;
  char buffer[CC_STRING_MIN_SIZE];
};

typedef  struct cc_string  cc_string;

/* ********************************************************************** */

COIN_DLL_API void cc_string_construct(cc_string * me);
COIN_DLL_API cc_string * cc_string_construct_new(void);
COIN_DLL_API cc_string * cc_string_clone(cc_string * str);
COIN_DLL_API void cc_string_clean(cc_string * str);
COIN_DLL_API void cc_string_destruct(cc_string * str);

COIN_DLL_API void cc_string_set_string(cc_string * str, const cc_string * str2);
COIN_DLL_API void cc_string_set_text(cc_string * str, const char * text);
COIN_DLL_API void cc_string_set_subtext(cc_string * str, const char * text, int start, int end);
COIN_DLL_API void cc_string_set_integer(cc_string * str, int integer);

COIN_DLL_API void cc_string_append_string(cc_string * str, const cc_string * str2);
COIN_DLL_API void cc_string_append_text(cc_string * str, const char * text);
COIN_DLL_API void cc_string_append_integer(cc_string * str, const int digits);
COIN_DLL_API void cc_string_append_char(cc_string * str, const char c);

COIN_DLL_API unsigned int cc_string_length(const cc_string * str);
COIN_DLL_API int cc_string_is(const cc_string * str);
COIN_DLL_API void cc_string_clear(cc_string * str);
COIN_DLL_API void cc_string_clear_no_free(cc_string * str);
COIN_DLL_API uint32_t cc_string_hash(const cc_string * str);
COIN_DLL_API uint32_t cc_string_hash_text(const char * text);
COIN_DLL_API const char * cc_string_get_text(const cc_string * str);
COIN_DLL_API void cc_string_remove_substring(cc_string * str, int start, int end);

COIN_DLL_API int cc_string_compare(const cc_string * lhs, const cc_string * rhs);
COIN_DLL_API int cc_string_compare_text(const char * lhs, const char * rhs);
COIN_DLL_API int cc_string_compare_subtext(const cc_string * str, const char * text, int offset);

COIN_DLL_API void cc_string_apply(cc_string * str, char (*function)(char input));

COIN_DLL_API void cc_string_sprintf(cc_string * str, const char * formatstr, ...);
COIN_DLL_API void cc_string_vsprintf(cc_string * str, const char * formatstr, va_list args);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_STRING_H */
