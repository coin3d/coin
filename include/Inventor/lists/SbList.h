#ifndef COIN_SBLIST_H
#define COIN_SBLIST_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
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

template <class Type>
class COIN_DLL_API SbList {
  // Older compilers aren't too happy about const declarations in the
  // class definitions, so use the enum trick described by Scott
  // Meyers in "Effective C++".
  enum { DEFAULTSIZE = 4 };

public:

  SbList(const int sizehint = DEFAULTSIZE)
    : itembuffersize(DEFAULTSIZE), numitems(0), itembuffer(builtinbuffer) {
    if (sizehint > DEFAULTSIZE) this->grow(sizehint);
  }

  SbList(const SbList<Type> & l)
    : itembuffersize(DEFAULTSIZE), numitems(0), itembuffer(builtinbuffer) {
    this->copy(l);
  }

  ~SbList() {
    if (this->itembuffer != builtinbuffer) delete[] this->itembuffer;
  }

  void copy(const SbList<Type> & l) {
    if (this == &l) return;
    const int n = l.numitems;
    this->expand(n);
    for (int i = 0; i < n; i++) this->itembuffer[i] = l.itembuffer[i];
  }

  SbList <Type> & operator=(const SbList<Type> & l) {
    this->copy(l);
    return *this;
  }

  void fit(void) {
    const int items = this->numitems;

    if (items < this->itembuffersize) {
      Type * newitembuffer = this->builtinbuffer;
      if (items > DEFAULTSIZE) newitembuffer = new Type[items];

      if (newitembuffer != this->itembuffer) {
        for (int i = 0; i < items; i++) newitembuffer[i] = this->itembuffer[i];
      }

      if (this->itembuffer != this->builtinbuffer) delete[] this->itembuffer;
      this->itembuffer = newitembuffer;
      this->itembuffersize = items > DEFAULTSIZE ? items : DEFAULTSIZE;
    }
  }

  void append(const Type item) {
    if (this->numitems == this->itembuffersize) this->grow();
    this->itembuffer[this->numitems++] = item;
  }

  int find(const Type item) const {
    for (int i = 0; i < this->numitems; i++)
      if (this->itembuffer[i] == item) return i;
    return -1;
  }

  void insert(const Type item, const int insertbefore) {
#ifdef COIN_EXTRA_DEBUG
    assert(insertbefore >= 0 && insertbefore <= this->numitems);
#endif // COIN_EXTRA_DEBUG
    if (this->numitems == this->itembuffersize) this->grow();

    for (int i = this->numitems; i > insertbefore; i--)
      this->itembuffer[i] = this->itembuffer[i-1];
    this->itembuffer[insertbefore] = item;
    this->numitems++;
  }

  void removeItem(const Type item) {
    int idx = this->find(item);
#ifdef COIN_EXTRA_DEBUG
    assert(idx != -1);
#endif // COIN_EXTRA_DEBUG
    this->remove(idx);
  }

  void remove(const int index) {
#ifdef COIN_EXTRA_DEBUG
    assert(index >= 0 && index < this->numitems);
#endif // COIN_EXTRA_DEBUG
    this->numitems--;
    for (int i = index; i < this->numitems; i++)
      this->itembuffer[i] = this->itembuffer[i + 1];
  }

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

  void push(const Type item) {
    this->append(item);
  }

  Type pop(void) {
#ifdef COIN_EXTRA_DEBUG
    assert(this->numitems > 0);
#endif // COIN_EXTRA_DEBUG
    return this->itembuffer[--this->numitems];
  }

  const Type * getArrayPtr(const int start = 0) const {
    return &this->itembuffer[start];
  }

  Type operator[](const int index) const {
#ifdef COIN_EXTRA_DEBUG
    assert(index >= 0 && index < this->numitems);
#endif // COIN_EXTRA_DEBUG
    return this->itembuffer[index];
  }

  Type & operator[](const int index) {
#ifdef COIN_EXTRA_DEBUG
    assert(index >= 0 && index < this->numitems);
#endif // COIN_EXTRA_DEBUG
    return this->itembuffer[index];
  }

  int operator==(const SbList<Type> & l) const {
    if (this == &l) return TRUE;
    if (this->numitems != l.numitems) return FALSE;
    for (int i = 0; i < this->numitems; i++)
      if (this->itembuffer[i] != l.itembuffer[i]) return FALSE;
    return TRUE;
  }

  int operator!=(const SbList<Type> & l) const {
    return !(*this == l);
  }

protected:

  void expand(const int size) {
    this->grow(size);
    this->numitems = size;
  }

  int getArraySize(void) const {
    return this->itembuffersize;
  }

private:
  void grow(const int size = -1) {
    // Default behavior is to double array size.
    if (size == -1) this->itembuffersize <<= 1;
    else if (size <= this->itembuffersize) return;
    else { this->itembuffersize = size; }

    Type * newbuffer = new Type[this->itembuffersize];
    const int n = this->numitems;
    for (int i = 0; i < n; i++) newbuffer[i] = this->itembuffer[i];
    if (this->itembuffer != this->builtinbuffer) delete[] this->itembuffer;
    this->itembuffer = newbuffer;
  }

  int itembuffersize;
  int numitems;
  Type * itembuffer;
  Type builtinbuffer[DEFAULTSIZE];
};

#endif // !COIN_SBLIST_H
