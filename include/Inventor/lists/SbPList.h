#ifndef COIN_LISTS_SBPLIST_H
#define COIN_LISTS_SBPLIST_H

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

class COIN_DLL_API SbPList : public SbList<void *> {
public:
  SbPList(void) : SbList<void *>() { }
  SbPList(const int sizehint) : SbList<void *>(sizehint) { }
  SbPList(const SbPList & l) : SbList<void *>(l) { }

  void * get(const int index) const { return (*this)[index]; }
  void set(const int index, void * const item) { (*this)[index] = item; }
  void * operator[](const int index) const;
  void * & operator[](const int index);

private:
  void expandlist(const int size);
};

// For compatibility with Open Inventor.
#ifndef COIN_INTERNAL
#include <Inventor/lists/SbIntList.h>
#include <Inventor/lists/SbVec3fList.h>
#include <Inventor/lists/SbStringList.h>
#endif // COIN_INTERNAL

#endif // !COIN_LISTS_SBPLIST_H
