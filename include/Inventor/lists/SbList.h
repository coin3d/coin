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

#ifndef __SBLIST_H__
#define __SBLIST_H__

#include <Inventor/SbBasic.h>
#include <stddef.h>
#include <assert.h>


template <class Type>
class SbList {
public:
  SbList(const int initsize = 4);
  SbList(const SbList<Type> & l);
  ~SbList();

  void copy(const SbList<Type> & l);

  void append(const Type item);
#if 0 // OBSOLETED: see comments by mortene in SbPList
  void append(const SbList<Type> & l);
#endif // OBSOLETED
  int find(const Type item) const;
  void insert(const Type item, const int insertbefore);
  void remove(const int index);
  void removeFast(const int index);
  int getLength(void) const;
  void truncate(const int length);
  void clear(const int newsize = 4);

  void push(const Type item);
  Type pop(void);

  SbList<Type> & operator = (const SbList<Type> & l);

  operator Type * (void);
  operator const Type * (void) const;
  const Type * constArrayPointer(void) const;
  Type * arrayPointer(void);

  Type operator [] (const int index) const;
  Type & operator [] (const int index);

  int operator == (const SbList<Type> & l) const;
  int operator != (const SbList<Type> & l) const;

  Type get(const int index) const;
  void set(const int index, const Type item);

  void fit(void);
  Type * stealPointer(void);

  void setBufferSize(const int newsize);

private:
  void growList(void);

  Type * itembuffer;
  int itembuffersize;
  int numitems;
};

template <class Type> inline void
SbList<Type>::growList(void)
{
  this->itembuffersize <<= 1;
  Type * newBuffer = new Type[this->itembuffersize];
  const int n = this->numitems;
  for (int i = 0; i < n; i++) {
    newBuffer[i] = this->itembuffer[i];
  }
  delete [] this->itembuffer;
  this->itembuffer = newBuffer;
}

template <class Type> inline
SbList<Type>::SbList(const int initsize)
  : numitems(0)
{
  this->itembuffersize = SbMax(4, initsize);
  this->itembuffer = new Type[initsize];
}

template <class Type> inline
SbList<Type>::SbList(const SbList<Type> & l)
  : numitems(0)
{
  const int n = l.numitems;
  this->itembuffersize = SbMax(4, n);
  this->numitems = n;
  this->itembuffer = new Type[this->itembuffersize];
  for (int i = 0; i < n; i++) this->itembuffer[i] = l.itembuffer[i];
}

template <class Type> inline
SbList<Type>::~SbList()
{
  delete [] this->itembuffer;
}

template <class Type> inline void
SbList<Type>::copy(const SbList<Type> & l)
{
  assert(this != &l); // can't do this - but can just return, though
  delete [] this->itembuffer;
  const int n = l.numitems;
  this->itembuffersize = SbMax(4,n);
  this->numitems = n;
  this->itembuffer = new Type[this->itembuffersize];
  for (int i = 0; i < n; i++) this->itembuffer[i] = l.itembuffer[i];
}

template <class Type> inline void
SbList<Type>::append(const Type item)
{
  if (this->numitems == this->itembuffersize) {
    this->growList();
  }
  this->itembuffer[this->numitems++] = item;
}

#if 0 // OBSLETED, see comments by mortene in SbPList
template <class Type> inline void
SbList<Type>::append(const SbList<Type> & l)
{

  assert(this != &l);
  const int items = l.getLength();
  for (int i = 0; i < items; i++)
    append(l.itembuffer[i]);
}
#endif // OBSOLETED

template <class Type> inline int
SbList<Type>::find(const Type item) const
{
  for (int i = 0; i < this->numitems; i++)
    if (this->itembuffer[i] == item)
      return i;
  return -1;
}

template <class Type> inline void
SbList<Type>::insert(const Type item, const int insertbefore)
{
  assert(insertbefore >= 0 && insertbefore <= this->numitems); // [0,N]
  if (this->numitems == this->itembuffersize) {
    this->growList();
  }
  for (int i = this->numitems; i > insertbefore; i--)
    this->itembuffer[i] = this->itembuffer[i-1];
  this->itembuffer[insertbefore] = item;
  this->numitems++;
}

template <class Type> inline void
SbList<Type>::remove(const int index)
{
  assert(index >= 0 && index < this->numitems); // range [0,N-1]
  this->numitems--;
  for (int i = index; i < this->numitems; i++)
    this->itembuffer[i] = this->itembuffer[i + 1];
}

template <class Type> inline void
SbList<Type>::removeFast(const int index)
{
  assert(index >= 0 && index < this->numitems); // range [0,N-1]
  this->itembuffer[index] = this->itembuffer[--this->numitems];
}

template <class Type> inline int
SbList<Type>::getLength(void) const
{
  return this->numitems;
}

template <class Type> inline void
SbList<Type>::truncate(const int length)
{
  assert(length <= this->numitems);
  this->numitems = length;
}

template <class Type> inline void
SbList<Type>::clear(const int newsize)
{
  this->numitems = 0;
  if (this->itembuffersize != newsize) {
    delete [] this->itembuffer;
    this->itembuffersize = SbMax(4, newsize);
    this->itembuffer = new Type[this->itembuffersize];
  }
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

template <class Type> inline
SbList<Type> &
SbList<Type>::operator = (const SbList<Type> & l)
{
  this->copy(l);
  return *this;
}

template <class Type> inline Type
SbList<Type>::operator [] (const int index) const
{
  assert(index >= 0 && index < this->numitems);
  return this->itembuffer[index];
}

template <class Type> inline Type &
SbList<Type>::operator [] (const int index)
{
#if 0 // OBSOLETED: dangerous. 19991025 mortene.
  assert(index >= 0);
  while (index >= this->itembuffersize) this->growList();
  if (index >= this->numitems) this->numitems = index+1;
#else // new code
  assert(index >= 0 && index < this->numitems);
#endif // new code
  return this->itembuffer[index];
}

template <class Type> inline SbBool
SbList<Type>::operator == (const SbList<Type> & l) const
{
  if (this == &l)
    return TRUE;
  if (this->numitems != l.numitems)
    return FALSE;
  for (int i = 0; i < this->numitems; i++)
    if (this->itembuffer[i] != l.itembuffer[i])
      return FALSE;
  return TRUE;
}

template <class Type> inline SbBool
SbList<Type>::operator != (const SbList<Type> & l) const
{
  if (this == &l)
    return FALSE;
  if (this->numitems != l.numitems)
    return TRUE;
  for (int i = 0; i < this->numitems; i++)
    if (this->itembuffer[ i ] != l.itembuffer[ i ])
      return TRUE;
  return FALSE;
}

template <class Type> inline const Type *
SbList<Type>::constArrayPointer(void) const
{
  return this->itembuffer;
}

template <class Type> inline Type *
SbList<Type>::arrayPointer(void)
{
  return this->itembuffer;
}

template <class Type> inline Type
SbList<Type>::get(const int index) const
{
  assert(index >= 0 && index < this->numitems); // range [0,N-1]
  return this->itembuffer[ index ];
}

template <class Type> inline void
SbList<Type>::set(const int index, const Type item)
{
  assert(index >= 0 && index < this->numitems); // range[ 0,N-1]
  this->itembuffer[index] = item;
}

template <class Type> inline void
SbList<Type>::fit(void)
{
  const int items = SbMax(this->numitems, 4);
  if (items < this->itembuffersize) {
    Type * newitembuffer = new Type[items];
    for (int i = 0; i < this->numitems; i++)
      newitembuffer[ i ] = this->itembuffer[i];
    delete [] this->itembuffer;
    this->itembuffer = newitembuffer;
    this->itembuffersize = items;
  }
}

template <class Type> inline Type *
SbList<Type>::stealPointer(void)
{
  Type * ptr = this->itembuffer;
  this->itembuffer = NULL;
  this->numitems = 0;
  this->itembuffersize = 0;
  return ptr;
}

template <class Type> inline void
SbList<Type>::setBufferSize(const int newsize)
{
  assert(newsize > 0);
  Type * newbuffer = new Type[newsize];

  // just skip items beyond new buffer
  if (newsize < this->numitems) this->numitems = newsize;

  for (int i = 0; i < this->numitems; i++) {
    newbuffer[i] = this->itembuffer[i];
  }
  delete [] this->itembuffer;
  this->itembuffer = newbuffer;
  this->itembuffersize = newsize;
}

#endif // !__SBLIST_H__
