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
  \class SoPickedPointList SoPickedPointList.h Inventor/lists/SoPickedPointList.h
  \brief The SoPickedPointList class is a container for pointers to SoPickedPoint objects.
  \ingroup general
*/

// SoPickedPointList was moved from being a subclass of SbPList to
// being a subclass of SbList. This removed the need to do lots of
// ugly casts in overloaded methods, with the subsequent removal of
// all code in this file. 20000228 mortene.

/*!
  \fn SoPickedPointList::SoPickedPointList(void)

  Default constructor.
*/

/*!
  \fn SoPickedPointList::SoPickedPointList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int sizehint)
*/

/*!
  \fn SoPickedPointList::SoPickedPointList(const SoPickedPointList & l)

  Copy constructor.

  \sa SbList::SbList(const SbList<Type> & l)
*/

/*!
  \fn void SoPickedPointList::set(const int index, SoPickedPoint * item)

  This method sets the element at \a index to \a item. Does the same
  thing as SbList::operator[](). This method is only present for
  compatibility with the original Inventor API.
*/
