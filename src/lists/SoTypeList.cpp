/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoTypeList SoTypeList.h Inventor/lists/SoTypeList.h
  \brief The SoTypeList class is a container class for arrays of SoType objects.
  \ingroup general

  \sa SbList
*/

// SoTypeList was moved from being a subclass of SbPList to being a
// subclass of SbList. This removed the need to do lots of ugly casts
// in overloaded methods, with the subsequent removal of all code in
// this file. 19991025 mortene.

/*!
  \fn SoTypeList::SoTypeList(void)

  Default constructor.
*/

/*!
  \fn SoTypeList::SoTypeList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int sizehint)
*/

/*!
  \fn SoTypeList::SoTypeList(const SoTypeList & l)

  Copy constructor.

  \sa SbList::SbList(const SbList<Type> & l)
*/

/*!
  \fn void SoTypeList::set(const int index, const SoType item)

  This method sets the element at \a index to \a item. Does the same
  thing as SbList::operator[](). This method is only present for
  compatibility with the original Inventor API.
*/
