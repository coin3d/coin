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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

cc_string * cc_string_construct(const char * string);
cc_string * cc_string_construct_new(void);
cc_string * cc_string_construct_substring(const char * string, unsigned int start, unsigned int end);
cc_string * cc_string_clone(cc_string * string);
void cc_string_destruct(cc_string * string);

uint32_t cc_string_hash(cc_string * string);
unsigned int cc_string_get_length(cc_string * string);
unsigned int cc_string_clear(cc_string * string);
const char * cc_string_get_string(cc_string * string);
cc_string * cc_string_get_substring(cc_string * string);


#if 0

class COIN_DLL_API SbString {
public:
  SbString(void);
  SbString(const char * str);
  SbString(const char * str, int start, int end);
  SbString(const SbString & str);
  SbString(const int digits);
  ~SbString();

  uint32_t hash(void);
  int getLength(void) const;
  void makeEmpty(SbBool freeold = TRUE);
  const char * getString(void) const;
  SbString getSubString(int startidx, int endidx = -1) const;
  void deleteSubString(int startidx, int endidx = -1);

  void addIntString(const int value);

  char operator [](int index) const;
  SbString & operator = (const char * str);
  SbString & operator = (const SbString & str);
  SbString & operator += (const char * str);
  SbString & operator += (const SbString & str);
  int operator ! (void) const;
  friend COIN_DLL_API int operator == (const SbString & str, const char * s);
  friend COIN_DLL_API int operator == (const char * s, const SbString & str);
  friend COIN_DLL_API int operator == (const SbString & str1, const SbString & str2);
  friend COIN_DLL_API int operator != (const SbString & str, const char * s);
  friend COIN_DLL_API int operator != (const char * s, const SbString & str);
  friend COIN_DLL_API int operator != (const SbString & str1, const SbString & str2);
  static uint32_t hash(const char * s);

  SbString & operator += (const char c);
  SbString & sprintf(const char * formatstr, ...);
  SbString & vsprintf(const char * formatstr, va_list args);

  void print(FILE * file = stdout) const;

private:
  char * sstring;
  int storagesize;
  char staticstorage[SB_STRING_STATIC_STORAGE_SIZE];
  void expand(int additional);
};

COIN_DLL_API int operator == (const SbString & str, const char * s);
COIN_DLL_API int operator == (const char * s, const SbString & str);
COIN_DLL_API int operator == (const SbString & str1, const SbString & str2);
COIN_DLL_API int operator != (const SbString & str, const char * s);
COIN_DLL_API int operator != (const char * s, const SbString & str);
COIN_DLL_API int operator != (const SbString & str1, const SbString & str2);

#endif

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_STRING_H */
