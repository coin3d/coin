#ifndef COIN_LISTS_SBPLIST_H
#define COIN_LISTS_SBPLIST_H

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

#include <Inventor/SbBasic.h>
#include <assert.h>
#include <stddef.h> // NULL definition

// We usually implement inline functions below the class definition,
// since we think that makes the file more readable. However, this is
// not done for this class, since Visual C++ is not too happy about
// having functions declared as inline for a template class.
// pederb, 2001-10-12

// FIXME: this is just a quick hack to avoid heaps of irritating
// warning messages from the compiler for client code compiled under
// MSVC++. Should try to find the real reason for the warnings and fix
// the cause of the problem instead. 20020730 mortene.
#ifdef _MSC_VER // Microsoft Visual C++
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#endif // _MSC_VER

class SbPList {
  // Older compilers aren't too happy about const declarations in the
  // class definitions, so use the enum trick described by Scott
  // Meyers in "Effective C++".
  enum { DEFAULTSIZE = 4 };

public:

  SbPList(const int sizehint = DEFAULTSIZE)
    : itembuffersize(DEFAULTSIZE), numitems(0), itembuffer(builtinbuffer) {
    if (sizehint > DEFAULTSIZE) this->grow(sizehint);
  }

  SbPList(const SbPList & l)
    : itembuffersize(DEFAULTSIZE), numitems(0), itembuffer(builtinbuffer) {
    this->copy(l);
  }

  ~SbPList();

  void copy(const SbPList & l);
  SbPList & operator=(const SbPList & l);
  void fit(void);

  void append(void * item) {
    if (this->numitems == this->itembuffersize) this->grow();
    this->itembuffer[this->numitems++] = item;
  }

  int find(void * item) const;
  void insert(void * item, const int insertbefore);
  void removeItem(void * item);
  void remove(const int index);
  void removeFast(const int index) {
#ifdef COIN_EXTRA_DEBUG
    assert(index >= 0 && index < this->numitems);
#endif // COIN_EXTRA_DEBUG
    this->itembuffer[index] = this->itembuffer[--this->numitems];
  }
  int getLength(void) const {
    return this->numitems;
  }

  void truncate(const int length, const int fit = 0) {
#ifdef COIN_EXTRA_DEBUG
    assert(length <= this->numitems);
#endif // COIN_EXTRA_DEBUG
    this->numitems = length;
    if (fit) this->fit();
  }

  void ** getArrayPtr(const int start = 0) const {
    return &this->itembuffer[start];
  }

  void *& operator[](const int index) const {
#ifdef COIN_EXTRA_DEBUG
    assert(index >= 0);
#endif // COIN_EXTRA_DEBUG
    if (index >= this->getLength()) this->expandlist(index + 1);
    return this->itembuffer[index];
  }

  int operator==(const SbPList & l) const;
  int operator!=(const SbPList & l) const {
    return !(*this == l);
  }
  
  void * get(const int index) const { return this->itembuffer[index]; }
  void set(const int index, void * item) { this->itembuffer[index] = item; }
  
protected:

  void expand(const int size) {
    this->grow(size);
    this->numitems = size;
  }
  int getArraySize(void) const {
    return this->itembuffersize;
  }

private:
  void expandlist(const int size) const; 
  void grow(const int size = -1);

  int itembuffersize;
  int numitems;
  void ** itembuffer;
  void * builtinbuffer[DEFAULTSIZE];
};

#endif // !COIN_LISTS_SBPLIST_H
