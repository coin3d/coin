/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SBVEC3FLIST_H
#define COIN_SBVEC3FLIST_H

#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec3f.h>


class SbVec3fList : public SbList<SbVec3f *> {
public:
  SbVec3fList(void) : SbList<SbVec3f *>() { }
  SbVec3fList(const int sizehint) : SbList<SbVec3f *>(sizehint) { }
  SbVec3fList(const SbVec3fList & l) : SbList<SbVec3f *>(l) { }

  void append(const SbVec3f * item) {
    SbVec3f * vec = new SbVec3f(*item);
    SbList<SbVec3f*>::append(vec);
  }

  void insert(const SbVec3f * item, const int insertbefore) {
    SbVec3f * vec = new SbVec3f(*item);
    SbList<SbVec3f*>::insert(vec, insertbefore);
  }

  SbVec3f * get(const int index) const { return (*this)[index]; }
  void set(const int index, SbVec3f * const item) { (*this)[index] = item; }
};

#endif // !COIN_SBVEC3FLIST_H
