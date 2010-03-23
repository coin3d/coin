#ifndef COIN_SBSTRINGLIST_H
#define COIN_SBSTRINGLIST_H

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

#include <Inventor/lists/SbPList.h>

class SbString;

class COIN_DLL_API SbStringList : public SbPList {
public:
  SbStringList(void) { }
  SbStringList(const int sizehint) : SbPList(sizehint) { }

  void append(SbString * string) {
    SbPList::append(static_cast<void *>(string));
  }
  int find(SbString * string) const {
    return SbPList::find(static_cast<void *>(string));
  }
  void insert(SbString * string, int insertbefore) {
    SbPList::insert(static_cast<void *>(string), insertbefore);
  }
  SbString *& operator[](const int idx) const {
    return reinterpret_cast<SbString*&> (this->SbPList::operator[](idx));
  }
  const SbString ** getArrayPtr(void) const {
    return const_cast<const SbString **>(reinterpret_cast<SbString **>(SbPList::getArrayPtr()));
  }
};

#endif // !COIN_SBSTRINGLIST_H
