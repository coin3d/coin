#ifndef COIN_SBSTRING_H
#define COIN_SBSTRING_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/system/inttypes.h>
#include <Inventor/C/base/string.h>
#include <Inventor/lists/SbIntList.h>

#include <stdarg.h>

class COIN_DLL_API SbString {
public:
  SbString(void) { cc_string_construct(&this->str); }

  SbString(const char * str)
  { cc_string_construct(&this->str); cc_string_set_text(&this->str, str); }

  SbString(const char * str, int start, int end)
  { cc_string_construct(&this->str); cc_string_set_subtext(&this->str, str, start, end); }

  SbString(const SbString & str)
  { cc_string_construct(&this->str); cc_string_set_string(&this->str, &str.str); }

  SbString(const int digits)
  { cc_string_construct(&this->str); cc_string_set_integer(&this->str, digits); }

  ~SbString() { cc_string_clean(&this->str); }

  uint32_t hash(void) { return cc_string_hash(&this->str); }
  static uint32_t hash(const char * s) { return cc_string_hash_text(s); }

  int getLength(void) const { return cc_string_length(&this->str); }

  void makeEmpty(SbBool freeold = TRUE)
  {
    if ( freeold ) cc_string_clear(&this->str);
    else cc_string_clear_no_free(&this->str);
  }

  const char * getString(void) const { return cc_string_get_text(&this->str); }

  SbString getSubString(int startidx, int endidx = -1) const
  {
    SbString s;
    cc_string_set_subtext(&s.str, cc_string_get_text(&this->str), startidx, endidx);
    return s;
  }
  void deleteSubString(int startidx, int endidx = -1)
  {
    cc_string_remove_substring(&this->str, startidx, endidx);
  }

  void addIntString(const int value) { cc_string_append_integer(&this->str, value); }

  char operator[](int index) const { return this->str.pointer[index]; }

  SbString & operator=(const char * str)
  { cc_string_set_text(&this->str, str); return *this; }
  SbString & operator=(const SbString & str)
  { cc_string_set_text(&this->str, str.str.pointer); return *this; }

  SbString & operator+=(const char * str)
  { cc_string_append_text(&this->str, str); return *this; }
  SbString & operator+=(const SbString & str)
  { cc_string_append_string(&this->str, &str.str); return *this; }
  SbString & operator+=(const char c)
  { cc_string_append_char(&this->str, c); return *this; }

  int operator!(void) const { return ! cc_string_is(&this->str); }

  int compareSubString(const char * text, int offset = 0) const
  { return cc_string_compare_subtext(&this->str, text, offset); }

  SbString & sprintf(const char * formatstr, ...)
  {
    va_list args; va_start(args, formatstr);
    cc_string_vsprintf(&this->str, formatstr, args);
    va_end(args); return *this;
  }
  SbString & vsprintf(const char * formatstr, va_list args)
  { cc_string_vsprintf(&this->str, formatstr, args); return *this; }

  void apply(char (*func)(char input)) { cc_string_apply(&this->str, (cc_apply_f)func); }

  int find(const SbString & str) const;
  SbBool findAll(const SbString & str, SbIntList & found) const;

  friend int operator==(const SbString & str, const char * s);
  friend int operator==(const char * s, const SbString & str);
  friend int operator==(const SbString & str1, const SbString & str2);
  friend int operator!=(const SbString & str, const char * s);
  friend int operator!=(const char * s, const SbString & str);
  friend int operator!=(const SbString & str1, const SbString & str2);

private:
  struct cc_string str;
};

inline int operator==(const SbString & str, const char * s)
{ return (cc_string_compare_text(str.str.pointer, s) == 0); }
inline int operator==(const char * s, const SbString & str)
{ return (cc_string_compare_text(s, str.str.pointer) == 0); }
inline int operator==(const SbString & str1, const SbString & str2)
{ return (cc_string_compare_text(str1.str.pointer, str2.str.pointer) == 0); }

inline int operator!=(const SbString & str, const char * s)
{ return (cc_string_compare_text(str.str.pointer, s) != 0); }
inline int operator!=(const char * s, const SbString & str)
{ return (cc_string_compare_text(s, str.str.pointer) != 0); }
inline int operator!=(const SbString & str1, const SbString & str2)
{ return (cc_string_compare_text(str1.str.pointer, str2.str.pointer) != 0); }

#ifndef COIN_INTERNAL
// For Open Inventor compatibility.
#include <Inventor/SbName.h>
#endif // !COIN_INTERNAL

#endif // !COIN_SBSTRING_H
