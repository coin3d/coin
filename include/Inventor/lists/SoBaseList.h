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

#ifndef COIN_SOBASELIST_H
#define COIN_SOBASELIST_H

class SoBase;

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbPList.h>


class SoBaseList : public SbPList {
public:
  SoBaseList(void);
  SoBaseList(const int size);
  SoBaseList(const SoBaseList & l);
  ~SoBaseList();

  void append(SoBase * ptr);
  void insert(SoBase * ptr, const int addbefore);
  void remove(const int index);
  void truncate(const int length);
  void copy(const SoBaseList & l);
  SoBaseList & operator=(const SoBaseList & l);
  SoBase * operator[](const int i) const;
  void set(const int i, SoBase * const ptr);
  void addReferences(const SbBool flag);

private:
  SbBool referencing;
};

#endif // !COIN_SOBASELIST_H
