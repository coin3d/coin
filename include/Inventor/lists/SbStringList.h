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

#ifndef COIN_SBSTRINGLIST_H
#define COIN_SBSTRINGLIST_H

#include <Inventor/lists/SbList.h>

class SbString;


class COIN_DLL_EXPORT SbStringList : public SbList<SbString *> {
public:
  SbStringList(void) : SbList<SbString *>() { }
  SbStringList(const int sizehint) : SbList<SbString *>(sizehint) { }
  SbStringList(const SbStringList & l) : SbList<SbString *>(l) { }

  SbString * get(const int index) const { return (*this)[index]; }
  void set(const int index, SbString * const item) { (*this)[index] = item; }
};

#endif // !COIN_SBSTRINGLIST_H
