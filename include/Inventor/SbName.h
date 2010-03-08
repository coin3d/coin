#ifndef COIN_SBNAME_H
#define COIN_SBNAME_H

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

#include <Inventor/SbBasic.h>

class SbString;

class COIN_DLL_API SbName {
public:
  SbName(void);
  SbName(const char * namestring);
  SbName(const SbString & str);
  SbName(const SbName & name);
  ~SbName();

  const char * getString(void) const;
  int getLength(void) const;

  int operator!(void) const;
  friend COIN_DLL_API int operator==(const SbName & lhs, const char * rhs);
  friend COIN_DLL_API int operator==(const char * lhs, const SbName & rhs);
  friend COIN_DLL_API int operator==(const SbName & lhs, const SbName & rhs);
  friend COIN_DLL_API int operator!=(const SbName & lhs, const char * rhs);
  friend COIN_DLL_API int operator!=(const char * lhs, const SbName & rhs);
  friend COIN_DLL_API int operator!=(const SbName & lhs, const SbName & rhs);

  operator const char * (void) const;

  // The following 4 functions shouldn't be in this class, they are
  // related solely to .iv/.wrl file I/O (and so would likely be
  // better placed in SoOutput). This is a design flaw, but we can't
  // fix it without breaking API compatibility with the original SGI
  // Inventor.
  static SbBool isIdentStartChar(const char c);
  static SbBool isIdentChar(const char c);
  static SbBool isBaseNameStartChar(const char c);
  static SbBool isBaseNameChar(const char c);

  static const SbName & empty(void);

private:
  const char * permaaddress;
};

COIN_DLL_API int operator==(const SbName & lhs, const char * rhs);
COIN_DLL_API int operator==(const char * lhs, const SbName & rhs);
COIN_DLL_API int operator==(const SbName & lhs, const SbName & rhs);
COIN_DLL_API int operator!=(const SbName & lhs, const char * rhs);
COIN_DLL_API int operator!=(const char * lhs, const SbName & rhs);
COIN_DLL_API int operator!=(const SbName & lhs, const SbName & rhs);

#endif // !COIN_SBNAME_H
