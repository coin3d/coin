#ifndef COIN_SBSTRING_H
#define COIN_SBSTRING_H

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

#include <Inventor/system/inttypes.h>
#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbIntList.h>

#include <stdio.h>
#include <stdarg.h>

#define SB_STRING_STATIC_STORAGE_SIZE 128


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

  int find(const SbString & str) const;
  SbBool findAll(const SbString & str, SbIntList & found) const;

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

#ifndef COIN_INTERNAL
// For Open Inventor compatibility.
#include <Inventor/SbName.h>
#endif // !COIN_INTERNAL

#endif // !COIN_SBSTRING_H
