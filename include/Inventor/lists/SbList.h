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
private:
  Type * itemBuffer;
  int itemBufferSize;
  int numItems;

public:
  SbList(const int initSize = 4);
  SbList(const SbList<Type> & list);
  ~SbList();

  void copy(const SbList<Type> & list);

  void append(const Type item);
#if 0 // OBSOLETED: see comments by mortene in SbPList
  void append(const SbList<Type> & list);
#endif // OBSOLETED
  int find(const Type item) const;
  void insert(const Type item, const int addBefore);
  void remove(const int index);
  void removeFast(const int index);
  int getLength(void) const;
  void truncate(const int length);
  void clear(const int newSize = 4);

  void push(const Type item);
  Type pop(void);

  SbList<Type> & operator = (const SbList<Type> & list);

  operator Type * (void);
  operator const Type * (void) const;
  const Type * constArrayPointer(void) const;
  Type * arrayPointer(void);

  Type operator [] (const int index) const;
  Type & operator [] (const int index);

  int operator == (const SbList<Type> & list) const;
  int operator != (const SbList<Type> & list) const;

  Type get(const int index) const;
  void set(const int index, const Type item);

  void fit(void);
  Type * stealPointer(void);

  void setBufferSize(const int newsize);

private:
  void growList(void);

};

template <class Type> inline void
SbList<Type>::growList()
{
  this->itemBufferSize <<= 1;
  Type * newBuffer = new Type[this->itemBufferSize];
  const int n = this->numItems;
  for (int i = 0; i < n; i++) {
    newBuffer[i] = this->itemBuffer[i];
  }
  delete [] this->itemBuffer;
  this->itemBuffer = newBuffer;
}

template <class Type> inline
SbList<Type>::SbList(const int initSize)
  : numItems(0)
{
  this->itemBufferSize = SbMax(4, initSize);
  this->itemBuffer = new Type[initSize];
}

template <class Type> inline
SbList<Type>::SbList(const SbList<Type> & list)
  : numItems(0)
{
  const int n = list.numItems;
  this->itemBufferSize = SbMax(4, n);
  this->numItems = n;
  this->itemBuffer = new Type[this->itemBufferSize];
  for (int i = 0; i < n; i++) this->itemBuffer[i] = list.itemBuffer[i];
}

template <class Type> inline
SbList<Type>::~SbList()
{
  delete [] this->itemBuffer;
}

template <class Type> inline void
SbList<Type>::copy(const SbList<Type> & list)
{
  assert(this != &list); // can't do this - but can just return, though
  delete [] this->itemBuffer;
  const int n = list.numItems;
  this->itemBufferSize = SbMax(4,n);
  this->numItems = n;
  this->itemBuffer = new Type[this->itemBufferSize];
  for (int i = 0; i < n; i++) this->itemBuffer[i] = list.itemBuffer[i];
}

template <class Type> inline void
SbList<Type>::append(const Type item)
{
  if (this->numItems == this->itemBufferSize) {
    this->growList();
  }
  this->itemBuffer[this->numItems++] = item;
}

#if 0 // OBSLETED, see comments by mortene in SbPList
template <class Type> inline void
SbList<Type>::append(const SbList<Type> & list)
{

  assert(this != &list);
  const int items = list.getLength();
  for (int i = 0; i < items; i++)
    append(list.itemBuffer[i]);
}
#endif // OBSOLETED

template <class Type> inline int
SbList<Type>::find(const Type item) const
{
  for (int i = 0; i < this->numItems; i++)
    if (this->itemBuffer[i] == item)
      return i;
  return -1;
}

template <class Type> inline void
SbList<Type>::insert(const Type item,
                     const int insertBefore)
{
  assert(insertBefore >= 0 && insertBefore <= this->numItems); // [0,N]
  if (this->numItems == this->itemBufferSize) {
    this->growList();
  }
  for (int i = this->numItems; i > insertBefore; i--)
    this->itemBuffer[i] = this->itemBuffer[i-1];
  this->itemBuffer[insertBefore] = item;
  this->numItems++;
}

template <class Type> inline void
SbList<Type>::remove(const int index)
{
  assert(index >= 0 && index < this->numItems); // range [0,N-1]
  this->numItems--;
  for (int i = index; i < this->numItems; i++)
    this->itemBuffer[i] = this->itemBuffer[i + 1];
}

template <class Type> inline void
SbList<Type>::removeFast(const int index)
{
  assert(index >= 0 && index < this->numItems); // range [0,N-1]
  this->itemBuffer[index] = this->itemBuffer[--this->numItems];
}

template <class Type> inline int
SbList<Type>::getLength() const
{
  return this->numItems;
}

template <class Type> inline void
SbList<Type>::truncate(const int length)
{
  assert(length <= this->numItems);
  this->numItems = length;
}

template <class Type> inline void
SbList<Type>::clear(const int newSize)
{
  this->numItems = 0;
  if (this->itemBufferSize != newSize) {
    delete [] this->itemBuffer;
    this->itemBufferSize = SbMax(4, newSize);
    this->itemBuffer = new Type[this->itemBufferSize];
  }
}

template <class Type> inline void
SbList<Type>::push(const Type item)
{
  append(item);
}

template <class Type> inline Type
SbList<Type>::pop()
{
  assert(this->numItems > 0);
  return this->itemBuffer[--this->numItems];
}

template <class Type> inline
SbList<Type>::operator Type *()
{
  return this->itemBuffer;
}

template <class Type> inline
SbList<Type>::operator const Type *() const
{
  return this->itemBuffer;
}

template <class Type> inline
SbList<Type> &
SbList<Type>::operator = (const SbList<Type> & list)
{
  copy(list);
  return *this;
}

template <class Type> inline Type
SbList<Type>::operator [] (const int index) const
{
  assert(index >= 0 && index < this->numItems);
  return this->itemBuffer[index];
}

template <class Type> inline Type &
SbList<Type>::operator [] (const int index)
{
#if 0 // OBSOLETED: dangerous. 19991025 mortene.
  assert(index >= 0);
  while (index >= this->itemBufferSize) this->growList();
  if (index >= this->numItems) this->numItems = index+1;
#else // new code
  assert(index >= 0 && index < this->numItems);
#endif // new code
  return this->itemBuffer[index];
}

template <class Type> inline SbBool
SbList<Type>::operator == (const SbList<Type> & list) const
{
  if (this == &list)
    return TRUE;
  if (this->numItems != list.numItems)
    return FALSE;
  for (int i = 0; i < this->numItems; i++)
    if (this->itemBuffer[i] != list.itemBuffer[i])
      return FALSE;
  return TRUE;
}

template <class Type> inline SbBool
SbList<Type>::operator != (const SbList<Type> & list) const
{
  if (this == &list)
    return FALSE;
  if (this->numItems != list.numItems)
    return TRUE;
  for (int i = 0; i < this->numItems; i++)
    if (this->itemBuffer[ i ] != list.itemBuffer[ i ])
      return TRUE;
  return FALSE;
}

template <class Type> inline const Type *
SbList<Type>::constArrayPointer() const
{
  return this->itemBuffer;
}

template <class Type> inline Type *
SbList<Type>::arrayPointer()
{
  return this->itemBuffer;
}

template <class Type> inline Type
SbList<Type>::get(const int index) const
{
  assert(index >= 0 && index < this->numItems); // range [0,N-1]
  return this->itemBuffer[ index ];
}

template <class Type> inline void
SbList<Type>::set(const int index,
                  const Type item)
{
  assert(index >= 0 && index < this->numItems); // range[ 0,N-1]
  this->itemBuffer[index] = item;
}

template <class Type> inline void
SbList<Type>::fit()
{
  const int items = SbMax(this->numItems, 4);
  if (items < this->itemBufferSize) {
    Type * newItemBuffer = new Type[items];
    for (int i = 0; i < this->numItems; i++)
      newItemBuffer[ i ] = this->itemBuffer[i];
    delete [] this->itemBuffer;
    this->itemBuffer = newItemBuffer;
    this->itemBufferSize = items;
  }
}

template <class Type> inline Type *
SbList<Type>::stealPointer()
{
  Type * ptr = this->itemBuffer;
  this->itemBuffer = NULL;
  this->numItems = 0;
  this->itemBufferSize = 0;
  return ptr;
}

template <class Type> inline void
SbList<Type>::setBufferSize(const int newsize)
{
  assert(newsize > 0);
  Type * newbuffer = new Type[newsize];

  // just skip items beyond new buffer
  if (newsize < this->numItems) this->numItems = newsize;

  for (int i = 0; i < this->numItems; i++) {
    newbuffer[i] = this->itemBuffer[i];
  }
  delete [] this->itemBuffer;
  this->itemBuffer = newbuffer;
  this->itemBufferSize = newsize;
}

#endif // !__SBLIST_H__
