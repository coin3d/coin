#ifndef COIN_SBINTLIST_H
#define COIN_SBINTLIST_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/lists/SbPList.h>

// *************************************************************************

class  COIN_DLL_API SbIntList : public SbPList {
public:
  SbIntList(void) : SbPList () { }
  SbIntList(const int sizehint) : SbPList(sizehint) { }
  
  void append(const int item) {
    ((SbPList*)this)->append((void*)((uintptr_t)item));
  }
  int find(const int item) {
    return ((SbPList*)this)->find((void *)((uintptr_t)item));
  }
  void insert(const int item, const int addbefore) {
    ((SbPList*)this)->insert((void *)((uintptr_t)item), addbefore);
  }
  int & operator[](const int idx) const {
    return (int&) ((*(const SbPList*)this)[idx]);
  }
};

// *************************************************************************

// Internally, we should use the SbList template class, since it
// provides a better interface than SbIntList, and since the interface
// of SbIntList may have issues with casting type safety.
//
// This class is provided for two reasons: 1) to be compatible with
// SGI/TGS Inventor, 2) there are a few places in the public API where
// it is used (these are all our own doing, not from the original SGI
// Inventor, and could be taken out for any new major release).

#if defined(COIN_INTERNAL) && !defined(COIN_ALLOW_SBINTLIST)
#error prefer SbList over SbIntList for internal code
#endif // COIN_INTERNAL

// *************************************************************************

#endif // !COIN_SBINTLIST_H
