/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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
