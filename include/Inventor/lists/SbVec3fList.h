#ifndef COIN_SBVEC3FLIST_H
#define COIN_SBVEC3FLIST_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec3f.h>

class COIN_DLL_API SbVec3fList : public SbList<SbVec3f *> {
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
