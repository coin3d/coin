#ifndef COIN_SOPICKEDPOINTLIST_H
#define COIN_SOPICKEDPOINTLIST_H

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

class SoPickedPoint;

class COIN_DLL_API SoPickedPointList : public SbPList {
public:
  SoPickedPointList(void) : SbPList() { }
  SoPickedPointList(const int sizehint) : SbPList(sizehint) { }
  SoPickedPointList(const SoPickedPointList & l);
  ~SoPickedPointList() { this->truncate(0); }
  
  void append(SoPickedPoint * pp) { 
    SbPList::append(static_cast<void *>(pp));
  }
  void insert(SoPickedPoint * pp, const int insertbefore) { 
    SbPList::insert(static_cast<void *>(pp), insertbefore);
  }
  SoPickedPoint * operator[](const int idx) const {
    return static_cast<SoPickedPoint *>(SbPList::operator[](idx));
  } 

  void truncate(const int start, const int fit = 0);
  void set(const int idx, SoPickedPoint * pp);

};

#endif // !COIN_SOPICKEDPOINTLIST_H
