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

#ifndef __SONODELIST_H__
#define __SONODELIST_H__

#include <Inventor/lists/SoBaseList.h>

class SoNode;

class SoNodeList : public SoBaseList {
  typedef SoBaseList inherited;
public:
  SoNodeList(void);
  SoNodeList(const int size);
  SoNodeList(const SoNodeList & l);
  ~SoNodeList(void);
  void append(SoNode * const ptr);
  SoNode * operator [](const int i) const;
  SoNodeList & operator =(const SoNodeList & l);

  //FIXME: Only used in SoChildList (non-OIV)
  void insert(SoNode * const node, const int addBefore);
};

#endif // !__SONODELIST_H__
