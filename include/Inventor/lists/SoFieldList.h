#ifndef COIN_SOFIELDLIST_H
#define COIN_SOFIELDLIST_H

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

class SoField;

class COIN_DLL_API SoFieldList : public SbPList {
public:
  SoFieldList(void) : SbPList() { }
  SoFieldList(const int sizehint) : SbPList(sizehint) { }
  SoFieldList(const SoFieldList & l) : SbPList(l) { }
  
  void append(SoField * field) { 
    SbPList::append((void *) field); 
  }
  void insert(SoField * field, const int insertbefore) {
    SbPList::insert((void *) field, insertbefore); 
  }

  SoField * operator [](const int idx) const {
    return (SoField*) SbPList::operator[](idx);
  }
  void set(const int idx, SoField * field) {
    SbPList::operator[](idx) = (void*) field;
  }
  SoField * get(const int idx) const {
    return (SoField*) SbPList::get(idx);
  }
};

#endif // !COIN_SOFIELDLIST_H
