#ifndef COIN_SOENGINEOUTPUTLIST_H
#define COIN_SOENGINEOUTPUTLIST_H

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

class SoEngineOutput;

class COIN_DLL_API SoEngineOutputList : public SbPList {
public:
  SoEngineOutputList(void) : SbPList() { }
  SoEngineOutputList(const int sizehint) : SbPList(sizehint) { }
  SoEngineOutputList(const SoEngineOutputList & l) : SbPList(l) { }

  void append(SoEngineOutput * output) {
    SbPList::append(static_cast<void *>(output));
  }
  void insert(SoEngineOutput * output, const int insertbefore) {
    SbPList::insert(static_cast<void *>(output), insertbefore);
  }
  SoEngineOutput * operator [](const int idx) const {
    return static_cast<SoEngineOutput *>(SbPList::operator[](idx));
  }
  void set(const int idx, SoEngineOutput * item) {
    SbPList::operator[](idx) = static_cast<void *>(item);
  }
};

#endif // !COIN_SOENGINEOUTPUTLIST_H
