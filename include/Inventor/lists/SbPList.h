#ifndef COIN_LISTS_SBPLIST_H
#define COIN_LISTS_SBPLIST_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>
#include <assert.h>
#include <stddef.h> // NULL definition

class COIN_DLL_API SbPList {
  enum { DEFAULTSIZE = 4 };

public:
  SbPList(const int sizehint = DEFAULTSIZE);
  SbPList(const SbPList & l);
  ~SbPList();

  void copy(const SbPList & l);
  SbPList & operator=(const SbPList & l);
  void fit(void);

  void append(void * item);
  int find(void * item) const;
  void insert(void * item, const int insertbefore);
  void removeItem(void * item);
  void remove(const int index);
  void removeFast(const int index);
  int getLength(void) const;
  void truncate(const int length, const int fit = 0);

  void ** getArrayPtr(const int start = 0) const;
  void *& operator[](const int index) const;

  int operator==(const SbPList & l) const;
  int operator!=(const SbPList & l) const;  
  void * get(const int index) const;
  void set(const int index, void * item);
  
protected:

  void expand(const int size);
  int getArraySize(void) const;

private:
  void expandlist(const int size) const; 
  void grow(const int size = -1);

  int itembuffersize;
  int numitems;
  void ** itembuffer;
  void * builtinbuffer[DEFAULTSIZE];
};

/* inlined methods ********************************************************/

inline void 
SbPList::append(void * item) 
{
  if (this->numitems == this->itembuffersize) this->grow();
  this->itembuffer[this->numitems++] = item;
}

inline void 
SbPList::removeFast(const int index) 
{
#ifdef COIN_EXTRA_DEBUG
  assert(index >= 0 && index < this->numitems);
#endif // COIN_EXTRA_DEBUG
  this->itembuffer[index] = this->itembuffer[--this->numitems];
}

inline int 
SbPList::getLength(void) const 
{
  return this->numitems;
}

inline void 
SbPList::truncate(const int length, const int dofit) 
{
#ifdef COIN_EXTRA_DEBUG
  assert(length <= this->numitems);
#endif // COIN_EXTRA_DEBUG
  this->numitems = length;
  if (dofit) this->fit();
}

inline void ** 
SbPList::getArrayPtr(const int start) const 
{
#ifdef COIN_EXTRA_DEBUG
  assert(index >= 0 && index < this->numitems);
#endif // COIN_EXTRA_DEBUG
  return &this->itembuffer[start];
}

inline void *& 
SbPList::operator[](const int index) const 
{
#ifdef COIN_EXTRA_DEBUG
  assert(index >= 0);
#endif // COIN_EXTRA_DEBUG
  if (index >= this->getLength()) this->expandlist(index + 1);
  return this->itembuffer[index];
}

inline int 
SbPList::operator!=(const SbPList & l) const 
{
  return !(*this == l);
}

inline void * 
SbPList::get(const int index) const 
{ 
  return this->itembuffer[index]; 
}

inline void 
SbPList::set(const int index, void * item) 
{ 
  this->itembuffer[index] = item; 
}

inline void 
SbPList::expand(const int size) 
{
  this->grow(size);
  this->numitems = size;
}

inline int 
SbPList::getArraySize(void) const 
{
  return this->itembuffersize;
}


#endif // !COIN_LISTS_SBPLIST_H
