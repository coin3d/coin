/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

#ifndef COIN_LISTS_SBPLIST_H
#define COIN_LISTS_SBPLIST_H

#include <Inventor/lists/SbList.h>

class COIN_DLL_EXPORT SbPList : public SbList<void *> {
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
