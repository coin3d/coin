/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SBINTLIST_H__
#define __SBINTLIST_H__

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbPList.h>

class SbIntList : public SbPList {
  typedef SbPList inherited;

public:
  SbIntList(const int initSize = 4);
  SbIntList(const SbIntList & list);
  ~SbIntList(void);

  void append(int integer);
  int find(int integer);
  void insert(int integer, int addBefore);
  void copy(const SbIntList & list);
  SbIntList & operator = (const SbIntList & list);
  int & operator [](int index) const;

  int get(const int index) const;
  void set(const int index, int integer);
  operator int * (void);
  operator const int * (void) const;
};

#endif // !__SBINTLIST_H__
