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

#ifndef __SOTYPELIST_H__
#define __SOTYPELIST_H__

#include <Inventor/lists/SbPList.h>

class SoType;

class SoTypeList : public SbPList {
  typedef SbPList inherited;
public:
  SoTypeList(void);
  SoTypeList(const int size);
  SoTypeList(const SoTypeList &l);
  ~SoTypeList(void);
  void append(SoType type);
  void insert(SoType type, const int addBefore);
  void set(const int i, SoType type);
  int find(SoType typeId) const;
  SoType &operator [](const int i) const;

  void setDefault(SoType type);
  SoType get(const int i) const;
};

#endif // !__SOTYPELIST_H__
