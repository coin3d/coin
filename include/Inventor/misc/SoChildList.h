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

#ifndef __SOCHILDLIST_H__
#define __SOCHILDLIST_H__

#include <Inventor/lists/SoNodeList.h>

class SoPath;
class SoAction;

class SoChildList : public SoNodeList {
  typedef SoNodeList inherited;
public:
  SoChildList(SoNode * const parent);
  SoChildList(SoNode * const parent, const int size);
  SoChildList(SoNode * const parent, const SoChildList & list);
  ~SoChildList(void);

  void append(SoNode * const node);
  void insert(SoNode * const ptr, const int addBefore);
  void remove(const int index);
  void truncate(const int length);
  void copy(const SoChildList & list);
  void set(const int index, SoNode * const node);

#if !defined(COIN_EXCLUDE_SOACTION)
  void traverse(SoAction * const action);
  void traverse(SoAction * const action, const int index);
  void traverse(SoAction * const action, const int first, const int last);
#endif // !COIN_EXCLUDE_SOACTION

  void addPathAuditor(SoPath * const path);
  void removePathAuditor(SoPath * const path);

private:
  SoNode * parent;
  // should not ref() paths...
  SbPList auditors;
};

//$ IMPORT INLINE ../../../src/misc/SoChildList.cpp

#endif // !__SOCHILDLIST_H__
