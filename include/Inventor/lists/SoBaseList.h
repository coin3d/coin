#ifndef COIN_SOBASELIST_H
#define COIN_SOBASELIST_H

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

class SoBase;

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbPList.h>

class COIN_DLL_API SoBaseList : public SbPList {
public:
  SoBaseList(void);
  SoBaseList(const int size);
  SoBaseList(const SoBaseList & l);
  ~SoBaseList();

  void append(SoBase * ptr);
  void insert(SoBase * ptr, const int addbefore);
  void remove(const int index);
  void truncate(const int length);
  void copy(const SoBaseList & l);
  SoBaseList & operator=(const SoBaseList & l);
  SoBase * operator[](const int i) const;
  void set(const int i, SoBase * const ptr);
  void addReferences(const SbBool flag);
  SbBool isReferencing(void) const;

private:
  SbBool referencing;
};

#endif // !COIN_SOBASELIST_H
