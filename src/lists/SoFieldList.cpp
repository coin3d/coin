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
  \class SoFieldList SoFieldList.h Inventor/lists/SoFieldList.h
  \brief The SoFieldList class is a container for pointers to SoField objects.
  \ingroup fields
*/

// SoFieldList was moved from being a subclass of SbPList to being a
// subclass of SbList. This removed the need to do lots of ugly casts
// in overloaded methods, with the subsequent removal of most of the
// code in this file. 20000228 mortene.

/*!
  \fn SoFieldList::SoFieldList(void)

  Default constructor.
*/

/*!
  \fn SoFieldList::SoFieldList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int)
*/

/*!
  \fn SoFieldList::SoFieldList(const SoFieldList & l)

  Copy constructor.

  \sa SbList::SbList(const SbList<Type> & l)
*/
