#ifndef COIN_SOAUDITORLIST_H
#define COIN_SOAUDITORLIST_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

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


class COIN_DLL_API SoAuditorList : private SbPList {
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
  void truncate(const int, const int = 0) { }
  void push(const void *) { }
  void * pop(void) { return NULL; }
  SbPList & operator=(const SbPList &) { return *this; }
  operator void ** (void) { return (void **) NULL; }
  operator const void ** (void) const { return (const void **) NULL; }
  void * operator[](const int) const { return NULL; }
  void * & operator[](const int) { return SbPList::operator[](0); }
  int operator==(const SbPList &) const { return 0; }
  int operator!=(const SbPList &) const { return 0; }

  void doNotify(SoNotList * l, const void * auditor, const SoNotRec::Type type);

};

#endif // !COIN_SOAUDITORLIST_H
