#ifndef COIN_SBVEC3FLIST_H
#define COIN_SBVEC3FLIST_H

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
#include <Inventor/SbVec3f.h>

class COIN_DLL_API SbVec3fList : public SbPList {
public:
  SbVec3fList(void) { }
  ~SbVec3fList();

  void append(const SbVec3f * item) {
    SbVec3f * vec = new SbVec3f(*item);
    
    ((SbPList*)this)->append((void*)vec);
  }

  void insert(const SbVec3f * item, const int insertbefore) {
    SbVec3f * vec = new SbVec3f(*item);
    ((SbPList*)this)->insert((void*)vec, insertbefore);
  }

  SbVec3f * operator[](const int idx) const {
    return (SbVec3f*) ((*(const SbPList*)this)[idx]);
  }
};

#endif // !COIN_SBVEC3FLIST_H
