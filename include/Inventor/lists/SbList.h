/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SBLIST_H
#define COIN_SBLIST_H

#include <Inventor/SbBasic.h>
#include <assert.h>
#include <stddef.h> // NULL definition


template <class Type>
class COIN_DLL_API SbList {
  // Older compilers aren't too happy about const declarations in the
  // class definitions, so use the enum trick described by Scott
  // Meyers in "Effective C++".
  enum { DEFAULTSIZE = 4 };

public:
  SbList(const int sizehint = DEFAULTSIZE);
  SbList(const SbList<Type> & l);

  // No need to define the destructor virtual, as Coin code should
  // always know the class type of the list objects it is handling.
  ~SbList();

  void copy(const SbList<Type> & l);

  void append(const Type item);
  int find(const Type item) const;
  void insert(const Type item, const int insertbefore);
  void remove(const int index);
  void removeFast(const int index);
  // Need to name this something else than remove(), to avoid
  // ambiguity when Type is "int".
  void removeItem(const Type item);

  int getLength(void) const;
  void truncate(const int length, const int fit = 0);

  void push(const Type item);
  Type pop(void);

  SbList<Type> & operator=(const SbList<Type> & l);

  const Type * getArrayPtr(const int start = 0) const;

  Type operator[](const int index) const;
  Type & operator[](const int index);

  int operator==(const SbList<Type> & l) const;
  int operator!=(const SbList<Type> & l) const;

  void fit(void);

protected:
  void expand(const int size);
  int getArraySize(void) const;

private:
  void grow(const int size = -1);

  int itembuffersize;
  int numitems;
  Type * itembuffer;
  Type builtinbuffer[DEFAULTSIZE];
};


/*** Inlined functions **********************************************/

template <class Type> inline
SbList<Type>::SbList(const int sizehint)
  : itembuffersize(DEFAULTSIZE), numitems(0), itembuffer(builtinbuffer)
{
  if (sizehint > DEFAULTSIZE) this->grow(sizehint);
}

template <class Type> inline
SbList<Type>::SbList(const SbList<Type> & l)
  : itembuffersize(DEFAULTSIZE), numitems(0), itembuffer(builtinbuffer)
{
  this->copy(l);
}

template <class Type> inline
SbList<Type>::~SbList()
{
  if (this->itembuffer != builtinbuffer) delete [] this->itembuffer;
}

// Increase array size without modifying the number of items in the
// array.
template <class Type> inline void
SbList<Type>::grow(const int size)
{
  // Default behavior is to double array size.
  if (size == -1) this->itembuffersize <<= 1;
  else if (size <= this->itembuffersize) return;
  else { this->itembuffersize = size; }

  Type * newbuffer = new Type[this->itembuffersize];
  const int n = this->numitems;
  for (int i = 0; i < n; i++) newbuffer[i] = this->itembuffer[i];
  if (this->itembuffer != this->builtinbuffer) delete [] this->itembuffer;
  this->itembuffer = newbuffer;
}

template <class Type> inline void
SbList<Type>::expand(const int size)
{
  this->grow(size);
  this->numitems = size;
}

template <class Type> inline int
SbList<Type>::getArraySize(void) const
{
  return this->itembuffersize;
}

template <class Type> inline void
SbList<Type>::copy(const SbList<Type> & l)
{
  if (this == &l) return;

  const int n = l.numitems;
  this->expand(n);
  for (int i = 0; i < n; i++) this->itembuffer[i] = l.itembuffer[i];
}

template <class Type> inline SbList<Type> &
SbList<Type>::operator=(const SbList<Type> & l)
{
  this->copy(l);
  return *this;
}

template <class Type> inline void
SbList<Type>::fit(void)
{
  const int items = this->numitems;

  if (items < this->itembuffersize) {
    Type * newitembuffer = this->builtinbuffer;
    if (items > DEFAULTSIZE) newitembuffer = new Type[items];

    if (newitembuffer != this->itembuffer) {
      for (int i = 0; i < items; i++) newitembuffer[i] = this->itembuffer[i];
    }

    if (this->itembuffer != this->builtinbuffer) delete [] this->itembuffer;
    this->itembuffer = newitembuffer;
    this->itembuffersize = items > DEFAULTSIZE ? items : DEFAULTSIZE;
  }
}

template <class Type> inline void
SbList<Type>::append(const Type item)
{
  if (this->numitems == this->itembuffersize) this->grow();
  this->itembuffer[this->numitems++] = item;
}

template <class Type> inline int
SbList<Type>::find(const Type item) const
{
  for (int i = 0; i < this->numitems; i++)
    if (this->itembuffer[i] == item) return i;
  return -1;
}

template <class Type> inline void
SbList<Type>::insert(const Type item, const int insertbefore)
{
#ifdef COIN_EXTRA_DEBUG
  assert(insertbefore >= 0 && insertbefore <= this->numitems);
#endif // COIN_EXTRA_DEBUG
  if (this->numitems == this->itembuffersize) this->grow();

  for (int i = this->numitems; i > insertbefore; i--)
    this->itembuffer[i] = this->itembuffer[i-1];
  this->itembuffer[insertbefore] = item;
  this->numitems++;
}

template <class Type> inline void
SbList<Type>::removeItem(const Type item)
{
  int idx = this->find(item);
#ifdef COIN_EXTRA_DEBUG
  assert(idx != -1);
#endif // COIN_EXTRA_DEBUG
  this->remove(idx);
}

template <class Type> inline void
SbList<Type>::remove(const int index)
{
#ifdef COIN_EXTRA_DEBUG 
  assert(index >= 0 && index < this->numitems);
#endif // COIN_EXTRA_DEBUG
  this->numitems--;
  for (int i = index; i < this->numitems; i++)
    this->itembuffer[i] = this->itembuffer[i + 1];
}

template <class Type> inline void
SbList<Type>::removeFast(const int index)
{
#ifdef COIN_EXTRA_DEBUG
  assert(index >= 0 && index < this->numitems);
#endif // COIN_EXTRA_DEBUG
  this->itembuffer[index] = this->itembuffer[--this->numitems];
}

template <class Type> inline int
SbList<Type>::getLength(void) const
{
  return this->numitems;
}

template <class Type> inline void
SbList<Type>::truncate(const int length, const int fit)
{
#ifdef COIN_EXTRA_DEBUG
  assert(length <= this->numitems);
#endif // COIN_EXTRA_DEBUG
  this->numitems = length;
  if (fit) this->fit();
}

template <class Type> inline void
SbList<Type>::push(const Type item)
{
  this->append(item);
}

template <class Type> inline Type
SbList<Type>::pop(void)
{
#ifdef COIN_EXTRA_DEBUG
  assert(this->numitems > 0);
#endif // COIN_EXTRA_DEBUG
  return this->itembuffer[--this->numitems];
}

template <class Type> inline const Type *
SbList<Type>::getArrayPtr(const int start) const
{
  return &this->itembuffer[start];
}

template <class Type> inline Type
SbList<Type>::operator[](const int index) const
{
#ifdef COIN_EXTRA_DEBUG
  assert(index >= 0 && index < this->numitems);
#endif // COIN_EXTRA_DEBUG
  return this->itembuffer[index];
}

template <class Type> inline Type &
SbList<Type>::operator[](const int index)
{
#ifdef COIN_EXTRA_DEBUG
  assert(index >= 0 && index < this->numitems);
#endif // COIN_EXTRA_DEBUG
  return this->itembuffer[index];
}

template <class Type> inline int
SbList<Type>::operator==(const SbList<Type> & l) const
{
  if (this == &l) return TRUE;
  if (this->numitems != l.numitems) return FALSE;
  for (int i = 0; i < this->numitems; i++)
    if (this->itembuffer[i] != l.itembuffer[i]) return FALSE;
  return TRUE;
}

template <class Type> inline int
SbList<Type>::operator!=(const SbList<Type> & l) const
{
  return !(*this == l);
}

#endif // !COIN_SBLIST_H
