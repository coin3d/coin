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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOAUDITORLIST_H__
#define __SOAUDITORLIST_H__

#include <Inventor/lists/SbPList.h>
#include <Inventor/misc/SoNotification.h>

// See Coin/build/base.pro for explanation.
#if defined(__SOLIB_MAKEASBASE__)
#define COIN_EXCLUDE_SODATASENSOR 1
#endif // __SOLIB_MAKEASBASE__

class SoAuditorList : private SbPList {
  typedef SbPList inherited;

public:
  SoAuditorList(void);
  ~SoAuditorList(void);
  
  void append(void * const auditor, const SoNotRec::Type type);
  void set(const int index, void * const auditor, const SoNotRec::Type type);
  int find(void * const auditor, const SoNotRec::Type type) const;

  void * getObject(const int index) const;
  SoNotRec::Type getType(const int index) const;

  void remove(const int index);
  void remove(void * const auditor, const SoNotRec::Type type);

  int getLength(void) const;
  void notify(SoNotList * list);
};

#endif // !__SOAUDITORLIST_H__
