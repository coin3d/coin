#ifndef COIN_SOFIELDLIST_H
#define COIN_SOFIELDLIST_H

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

class SoField;

class COIN_DLL_API SoFieldList : public SbPList {
public:
  SoFieldList(void) : SbPList() { }
  SoFieldList(const int sizehint) : SbPList(sizehint) { }
  SoFieldList(const SoFieldList & l) : SbPList(l) { }
  
  void append(SoField * field) { 
    SbPList::append(static_cast<void *>(field));
  }
  void insert(SoField * field, const int insertbefore) {
    SbPList::insert(static_cast<void *>(field), insertbefore);
  }

  SoField * operator [](const int idx) const {
    return static_cast<SoField *>(SbPList::operator[](idx));
  }
  void set(const int idx, SoField * field) {
    SbPList::operator[](idx) = static_cast<void *>(field);
  }
  SoField * get(const int idx) const {
    return static_cast<SoField*>(SbPList::get(idx));
  }
};

#endif // !COIN_SOFIELDLIST_H
