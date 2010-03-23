/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/lists/SoEngineOutputList.h>

/*!
  \class SoEngineOutputList SoEngineOutputList.h Inventor/lists/SoEngineOutputList.h
  \brief The SoEngineOutputList class is a container for SoEngineOutput objects.
  \ingroup engines

  \sa SbList
*/

/*!
  \fn SoEngineOutputList::SoEngineOutputList(void)

  Default constructor.
*/

/*!
  \fn SoEngineOutputList::SoEngineOutputList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.
*/

/*!
  \fn SoEngineOutputList::SoEngineOutputList(const SoEngineOutputList & l)

  Copy constructor.
*/

// void append(SoEngineOutput * output);
// void insert(SoEngineOutput * output, const int insertbefore);

/*!
  \fn SoEngineOutput * SoEngineOutput::operator[](const int idx) const

  Returns element at \a idx.

  Will automatically expand the size of the internal array if \a idx
  is outside the current bounds of the list. The values of any
  additional pointers are then set to \c NULL.
*/


/*!
  \fn void SoEngineOutputList::set(const int index, SoEngineOutput * item)
*/
