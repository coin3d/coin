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

/*!
  \class SbStringList SbStringList.h Inventor/lists/SbStringList.h
  \brief The SbStringList class is a container for arrays of SbString pointers.
  \ingroup base

  Note that upon using the equality and inequality operators, the
  strings themselves are not compared, only the pointer values.

  \sa SbList
*/


/*!
  \fn SbStringList::SbStringList(void)

  Default constructor.
*/

/*!
  \fn SbStringList::SbStringList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int sizehint)
*/

/*!
  \fn SbStringList::SbStringList(const SbStringList & l)

  Copy constructor.

  \sa SbList::SbList(const SbList<Type> & l)
*/

/*!
  \fn SbString * SbStringList::get(const int index) const

  This method returns the element at \a index. Does the same thing as
  SbList::operator[](). This method is only present for compatibility
  with the original Inventor API.
*/

/*!
  \fn void SbStringList::set(const int index, SbString * const item)

  This method sets the element at \a index to \a item. Does the same
  thing as SbList::operator[](). This method is only present for
  compatibility with the original Inventor API.
*/
