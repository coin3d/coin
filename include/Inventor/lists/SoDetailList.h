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

#ifndef _SO_DETAIL_LIST_H_
#define _SO_DETAIL_LIST_H_

#include <Inventor/lists/SbPList.h>

class SoDetail;

class SoDetailList : public SbPList {
  typedef SbPList inherited;
public:
  SoDetailList(void);
  SoDetailList(const int size);
  SoDetailList(const SoDetailList &l);
  ~SoDetailList(void);
  void append(SoDetail * const ptr);
  void insert(SoDetail * const ptr, const int addBefore);
  void truncate(const int start);
  void copy(const SoDetailList &l);
  SoDetailList &operator =(const SoDetailList &l);
  SoDetail *operator [](const int i) const;
  void set(const int i, SoDetail * const ptr);
};

#endif // ! _SO_DETAIL_LIST_H_
