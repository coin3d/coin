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

#ifndef COIN_SBLIST_H
#define COIN_SBLIST_H

#include <Inventor/SbBasic.h>
#include <assert.h>
#include <stddef.h> // NULL definition


template <class Type>
class SbList {
  const int DEFAULTSIZE = 4;

public:
  SbList(const int sizehint = DEFAULTSIZE);
  SbList(const SbList<Type> & l);
  ~SbList();

  void copy(const SbList<Type> & l);

  void append(const Type item);
  int find(const Type item) const;
  void insert(const Type item, const int insertbefore);
  void remove(const int index);
  void removeFast(const int index);
  int getLength(void) const;
  void truncate(const int length, const int fit = 0);

  void push(const Type item);
  Type pop(void);

  SbList<Type> & operator=(const SbList<Type> & l);

  operator Type * (void);
  operator const Type * (void) const;

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
    this->itembuffersize = items;
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
  assert(insertbefore >= 0 && insertbefore <= this->numitems);
  if (this->numitems == this->itembuffersize) this->grow();

  for (int i = this->numitems; i > insertbefore; i--)
    this->itembuffer[i] = this->itembuffer[i-1];
  this->itembuffer[insertbefore] = item;
  this->numitems++;
}

template <class Type> inline void
SbList<Type>::remove(const int index)
{
  assert(index >= 0 && index < this->numitems);
  this->numitems--;
  for (int i = index; i < this->numitems; i++)
    this->itembuffer[i] = this->itembuffer[i + 1];
}

template <class Type> inline void
SbList<Type>::removeFast(const int index)
{
  assert(index >= 0 && index < this->numitems);
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
  assert(length <= this->numitems);
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
  assert(this->numitems > 0);
  return this->itembuffer[--this->numitems];
}

template <class Type> inline
SbList<Type>::operator Type *(void)
{
  return this->itembuffer;
}

template <class Type> inline
SbList<Type>::operator const Type *(void) const
{
  return this->itembuffer;
}

template <class Type> inline Type
SbList<Type>::operator[](const int index) const
{
  assert(index >= 0 && index < this->numitems);
  return this->itembuffer[index];
}

template <class Type> inline Type &
SbList<Type>::operator[](const int index)
{
  assert(index >= 0 && index < this->numitems);
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
