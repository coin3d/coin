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
  \class SbPList SbPList.h Inventor/lists/SbPList.h
  \brief The SbPList class is a container class for void pointers.
  \ingroup base

  This class is for most purposes superseded by the Coin-specific
  SbList class.
*/


#include <Inventor/lists/SbPList.h>

/*!
  \fn SbPList::SbPList(void)

  Default constructor.
*/

/*!
  \fn SbPList::SbPList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList<Type>::SbList(const int sizehint)
*/

/*!
  \fn SbPList::SbPList(const SbPList & l)
  Copy constructor. 

  \sa SbList<Type>::SbList(const SbList<Type> & l)
*/

/*!
  \fn void * SbPList::get(const int index) const

  This method returns the element at \a index. Does the same thing as
  the index operator. This method is only present for compatibility
  with the original Inventor API.
*/

/*!
  \fn void SbPList::set(const int index, void * const item)

  This method sets the element at \a index to \a item. Does the same
  thing as the assignment index operator. This method is only present
  for compatibility with the original Inventor API.
*/
