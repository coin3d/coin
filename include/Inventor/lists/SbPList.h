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

#ifndef COIN_LISTS_SBPLIST_H
#define COIN_LISTS_SBPLIST_H

// NB: make sure the ifdef-test above wrapping this includefile is
// _not_ checking on and setting the same define-string as the other
// SbPList.h file in the include/Inventor/ directory.

#include <stdio.h>


class SbPList {
public:
  SbPList(const int initSize = 4);
  SbPList(const SbPList & list);
  ~SbPList();

  void append(void * const item);
  int find(void * const item) const;
  void insert(void * const item, int addBefore);
  void remove(int index);
  int getLength() const;
  void truncate(const int length);
  void copy(const SbPList & list);
  SbPList & operator = (const SbPList & list);
  void * & operator [](int index) const;
  int operator == (const SbPList & list) const;
  int operator != (const SbPList & list) const;

  void * get(const int index) const;
  void set(const int index, void * const item);

//  void * operator [](const int index) const;
  void setDefault(const void * const defaultItem);
  const void * getDefault() const;
  // FIXME: I don't think this method is a very good idea -- it can
  // (and has) lead to _extremely_ hard to find bugs. egcs v1.0.2 at
  // least managed to read SbPList::append((uint32_t)0) as a call to
  // this method, resulting in a bogus SbPList constructor
  // call. 19990418 mortene.
#if 0 // disabled
  void append(const SbPList & list);
#endif // disabled
  void removeFast(int index);
  void clear(const int newSize = 4);
  void push(void * const item);
  void * pop();
  operator void **();
  operator const void **() const;
  void * getLastElement() const;
  void setLastElement(void * const item);
  void fit();

  void print(FILE * file = stdout) const;

protected:
  void makeRoom(int newSize);
  void makeDefaultRoom(int newSize) const;
  void **itemBuffer;
  int itemBufferSize;
  int numItems;
  void * defaultItem;
};

#ifndef COIN_INTERNAL
#define COIN_INCLUDED_FROM_SBPLIST_H
#include <Inventor/lists/SbStringList.h>
#undef COIN_INCLUDED_FROM_SBPLIST_H
#endif // COIN_INTERNAL

#endif // !COIN_LISTS_SBPLIST_H
