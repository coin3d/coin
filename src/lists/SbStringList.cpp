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

/*!
  \class SbStringList Inventor/lists/SbStringList.h
  \brief The SbStringList class is a container class for arrays of SbString
  objects.

  FIXME: write doc.
*/

#include <Inventor/lists/SbStringList.h>

/*!
  FIXME: write doc.
*/

void
SbStringList::append(SbString * string)
{
  ((SbPList *)this)->append((void *)string);
}

/*!
  FIXME: write doc.

  PS: the string value is not what is searched for - it is the string object.
*/

int
SbStringList::find(SbString * string) const
{
  return ((SbPList *)this)->find((void *)string);
}

/*!
  FIXME: write doc.
*/

void
SbStringList::insert(SbString * string, const int addBefore)
{
  ((SbPList *)this)->insert((void *)string, addBefore);
}

/*!
  FIXME: write doc.
*/

SbString *&
SbStringList::operator [] (const int i) const
{
  return (SbString *&)(*(SbPList *)this)[i];
}
