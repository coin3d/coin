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

#ifndef COIN_SOAUDITORLIST_H
#define COIN_SOAUDITORLIST_H

#include <Inventor/lists/SbPList.h>
#include <Inventor/misc/SoNotification.h>


// Important note: do not expand this class with any further data
// unless absolutely necessary, as every SoBase object contains (by
// value) an SoAuditorList.
//
// So if you increase the size of objects of this class by just a
// minor amount, it could have adverse effects on the total memory
// usage in a Coin application, as SoBase-derived objects are
// ubiquitous within the system.
//
//  -mortene


class SoAuditorList : private SbPList {
  typedef SbPList inherited;

public:
  SoAuditorList(void);
  ~SoAuditorList();

  void append(void * const auditor, const SoNotRec::Type type);

  void set(const int index, void * const auditor, const SoNotRec::Type type);
  void * getObject(const int index) const;
  SoNotRec::Type getType(const int index) const;

  int getLength(void) const;
  int find(void * const auditor, const SoNotRec::Type type) const;

  void remove(const int index);
  void remove(void * const auditor, const SoNotRec::Type type);

  void notify(SoNotList * l);

private:
  // Hide these, as they are "dangerous" for this class, in the sense
  // that they need to be rewritten to behave correctly.
  SoAuditorList(const int) { }
  SoAuditorList(const SoAuditorList & l) : SbPList(l) { }
  void * get(const int) const { return NULL; }
  void set(const int, void * const) { }
  void copy(const SbPList &) { }
  void append(const void *) { }
  int find(const void *) const { return -1; }
  void insert(const void *, const int) { }
  void removeFast(const int) { }
  void truncate(const int length, const int fit = 0) { }
  void push(const void *) { }
  void * pop(void) { return NULL; }
  SbPList & operator=(const SbPList &) { return *this; }
  operator void * * (void) { return NULL; }
  operator const void * * (void) const { return NULL; }
  void * operator[](const int) const { return NULL; }
  void * & operator[](const int) { return SbPList::operator[](0); }
  int operator==(const SbPList &) const { return 0; }
  int operator!=(const SbPList &) const { return 0; }
};

#endif // !COIN_SOAUDITORLIST_H
