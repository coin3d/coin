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

#ifndef __SBSTRINGLIST_H__
#define __SBSTRINGLIST_H__

class SbString;

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbPList.h>

class SbStringList : public SbPList {
  typedef SbPList inherited;

public:
  void append(SbString * string);
  int find(SbString * string) const;
  void insert(SbString * string, const int addBefore);
  SbString * & operator [] (const int i) const;
};

#endif // !__SBSTRINGLIST_H__
