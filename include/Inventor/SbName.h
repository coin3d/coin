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

#ifndef COIN_SBNAME_H
#define COIN_SBNAME_H

#include <Inventor/SbBasic.h>

#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif

class SbString;
struct SbNameChunk;

class COIN_DLL_EXPORT SbNameEntry {
  friend class SbName;
public:
  SbBool isEmpty(void) const
    { return (! *str); }
  SbBool isEqual(const char * const string) const
    { return (strcmp(str, string) == 0); }

private:
  static int nameTableSize;
  static SbNameEntry * * nameTable;
  static struct SbNameChunk * chunk;
  const char * str;
  unsigned long hashValue;
  SbNameEntry * next;

  static void initClass(void);
  SbNameEntry(const char * const s, const unsigned long h,
               SbNameEntry * const n)
    { str = s; hashValue = h; next = n; };
  static const SbNameEntry * insert(const char * const s);

  static const char * findStringAddress(const char * s);

public:
  static void print_info();
};

class COIN_DLL_EXPORT SbName {
public:
  SbName(void);
  SbName(const char * nameString);
  SbName(const SbString & str);
  SbName(const SbName & name);
  ~SbName(void);

  const char * getString(void) const;
  int getLength(void) const;
  static SbBool isIdentStartChar(const char c);
  static SbBool isIdentChar(const char c);
  static SbBool isBaseNameStartChar(const char c);
  static SbBool isBaseNameChar(const char c);

  int operator ! (void) const;
  friend COIN_DLL_EXPORT int operator == (const SbName & lhs, const char * rhs);
  friend COIN_DLL_EXPORT int operator == (const char * lhs, const SbName & rhs);
  friend COIN_DLL_EXPORT int operator == (const SbName & lhs, const SbName & rhs);
  friend COIN_DLL_EXPORT int operator != (const SbName & lhs, const char * rhs);
  friend COIN_DLL_EXPORT int operator != (const char * lhs, const SbName & rhs);
  friend COIN_DLL_EXPORT int operator != (const SbName & lhs, const SbName & rhs);

  operator const char * (void) const;

private:
  const SbNameEntry * entry;
};
COIN_DLL_EXPORT int operator == (const SbName & lhs, const char * rhs);
COIN_DLL_EXPORT int operator == (const char * lhs, const SbName & rhs);
COIN_DLL_EXPORT int operator == (const SbName & lhs, const SbName & rhs);
COIN_DLL_EXPORT int operator != (const SbName & lhs, const char * rhs);
COIN_DLL_EXPORT int operator != (const char * lhs, const SbName & rhs);
COIN_DLL_EXPORT int operator != (const SbName & lhs, const SbName & rhs);

#endif // !COIN_SBNAME_H
