#ifndef COIN_SBINTLIST_H
#define COIN_SBINTLIST_H

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

#include <Inventor/lists/SbPList.h>

class  COIN_DLL_API SbIntList : public SbPList {
public:
  SbIntList(void) : SbPList () { }
  SbIntList(const int sizehint) : SbPList(sizehint) { }
  
  void append(const int item) {
    ((SbPList*)this)->append((void*)((unsigned long)item));
  }
  int find(const int item) {
    return ((SbPList*)this)->find((void *)((unsigned long)item));
  }
  void insert(const int item, const int addbefore) {
    ((SbPList*)this)->insert((void *)((unsigned long)item), addbefore);
  }
  int & operator[](const int idx) const {
    return (int&) ((*(const SbPList*)this)[idx]);
  }
};

#endif // !COIN_SBINTLIST_H
