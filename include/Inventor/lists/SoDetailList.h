#ifndef COIN_SODETAILLIST_H
#define COIN_SODETAILLIST_H

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

class SoDetail;

class COIN_DLL_API SoDetailList : public SbPList {
public:
  SoDetailList(void) : SbPList() { }
  SoDetailList(const int sizehint) : SbPList (sizehint) { }
  SoDetailList(const SoDetailList & l);
  ~SoDetailList();

  void append(SoDetail * detail) {
    SbPList::append((void*) detail);
  }
  void insert(SoDetail * detail, const int insertbefore) {
    SbPList::insert((void*) detail, insertbefore);
  }
  void truncate(const int length, const int fit = 0); 
  void copy(const SoDetailList & l);
  SoDetailList & operator=(const SoDetailList & l) {
    this->copy(l);
    return *this;
  }
  SoDetail * operator[](const int idx) const {
    return (SoDetail*) ((*(const SbPList*)this)[idx]);
  }
  void set(const int index, SoDetail * item);
};

#endif // !COIN_SODETAILLIST_H
