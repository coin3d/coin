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

#ifndef COIN_SBNAME_H
#define COIN_SBNAME_H

#include <Inventor/SbBasic.h>

class SbString;

class COIN_DLL_API SbName {
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
  friend COIN_DLL_API int operator == (const SbName & lhs, const char * rhs);
  friend COIN_DLL_API int operator == (const char * lhs, const SbName & rhs);
  friend COIN_DLL_API int operator == (const SbName & lhs, const SbName & rhs);
  friend COIN_DLL_API int operator != (const SbName & lhs, const char * rhs);
  friend COIN_DLL_API int operator != (const char * lhs, const SbName & rhs);
  friend COIN_DLL_API int operator != (const SbName & lhs, const SbName & rhs);

  operator const char * (void) const;

private:
  const class SbNameEntry * entry;
};
COIN_DLL_API int operator == (const SbName & lhs, const char * rhs);
COIN_DLL_API int operator == (const char * lhs, const SbName & rhs);
COIN_DLL_API int operator == (const SbName & lhs, const SbName & rhs);
COIN_DLL_API int operator != (const SbName & lhs, const char * rhs);
COIN_DLL_API int operator != (const char * lhs, const SbName & rhs);
COIN_DLL_API int operator != (const SbName & lhs, const SbName & rhs);

#endif // !COIN_SBNAME_H
