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

/*!
  \class SbIntList SbIntList.h Inventor/lists/SbIntList.h
  \brief The SbIntList class is a container class for integer list arrays.

  See class SbPList, from which SbIntList is derived.
*/

#include <Inventor/lists/SbIntList.h>

/*!
  A constructor.
*/

SbIntList::SbIntList(const int initSize):
  SbPList(initSize)
{
}

/*!
  A constructor.
*/

SbIntList::SbIntList(const SbIntList & list):
  SbPList((SbPList &)list)
{
}

/*!
  The destructor.
*/

SbIntList::~SbIntList()
{
}

/*!
  This method appends an integer to the end of the list.
*/

void
SbIntList::append(int integer)
{
  SbPList::append((void *)integer);
}

/*!
  This method finds the first index for an element with value \a integer.
*/

int
SbIntList::find(int integer)
{
  return SbPList::find((void *)integer);
}

/*!
  This method inserts the integer \a integer at position \a addBefore, bumping
  all the following values out.
*/

void
SbIntList::insert(int integer, int addBefore)
{
  SbPList::insert((void *)integer, addBefore);
}

/*!
  This method copies the contents of \a list into the instance.
*/

void
SbIntList::copy(const SbIntList & list)
{
  SbPList::copy((SbPList &)list);
}

/*!
  This operator copies the contents of \a list into the LHS instance,
  returning itself for further manipulation.
*/

SbIntList &
SbIntList::operator=(const SbIntList & list)
{
  return (SbIntList &)((*(SbPList *)this)=(SbPList &)list);
}

/*!
  This operator returns a reference to the integer at index \a index.
*/

int &
SbIntList::operator [](int index) const
{
  return (int &) (*(SbPList *)this)[index];
}

/*!
  This method returns the value of the integer at position \a index.
*/

int
SbIntList::get(const int index) const
{
  return (int)SbPList::get(index);
}

/*!
  This method sets the value of the integer at position \a index
  to the value \a integer.
*/

void
SbIntList::set(const int index, int integer)
{
  SbPList::set(index, (void *)integer);
}

/*!
  This method returns a pointer to the array as an int * pointer.

  FIXME: this will cause big problems on architectures with 64bit pointers
  and 32-bit integers.
*/


SbIntList::operator int * (void)
{
  return (int *)this->itemBuffer;
}

/*!
  This method returns a const pointer to the array as an int * pointer.

  FIXME: this will cause big problems on architectures with 64bit pointers
  and 32-bit integers.
*/

SbIntList::operator const int * (void) const
{
  return (const int *) this->itemBuffer;
}
