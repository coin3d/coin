/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLInterpOutputList SoVRMLInterpOutputList.h Inventor/lists/SoVRMLInterpOutputList.h
  \brief The SoVRMLInterpOutputList class is a container for SoVRMLInterpOutput objects.
  \ingroup vrml2
*/

// SoVRMLInterpOutputList was moved from being a subclass of SbPList to
// being a subclass of SbList. This removed the need to do lots of
// ugly casts in overloaded methods, with the subsequent removal of
// most of the code in this file. 20000228 mortene.

/*!
  \fn SoVRMLInterpOutputList::SoVRMLInterpOutputList(void)

  Default constructor.
*/

/*!
  \fn SoVRMLInterpOutputList::SoVRMLInterpOutputList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int)
*/

/*!
  \fn SoVRMLInterpOutputList::SoVRMLInterpOutputList(const SoVRMLInterpOutputList & l)

  Copy constructor.

  \sa SbList::SbList(const SbList<Type> & l)
*/
