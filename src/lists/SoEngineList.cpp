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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoEngineList SoEngineList.h Inventor/lists/SoEngineList.h
  \brief The SoEngineList class is a container for SoEngine objects.
  \ingroup engines

  As this class inherits SoBaseList, referencing and dereferencing
  will default be done on the objects at append(), remove(), insert()
  etc.
*/

#include <Inventor/lists/SoEngineList.h>


/*!
  Default constructor.
*/
SoEngineList::SoEngineList(void)
{
}

/*!
  Constructor with a hint about the number of elements the list will
  hold.

  \sa SoBaseList::SoBaseList(const int)
*/
SoEngineList::SoEngineList(const int size)
  : SoBaseList(size)
{
}

/*!
  Copy constructor.

  \sa SoBaseList::SoBaseList(const SoBaseList &)
*/
SoEngineList::SoEngineList(const SoEngineList & el)
  : SoBaseList(el)
{
}

/*!
  Destructor.

  \sa SoBaseList::~SoBaseList()
*/
SoEngineList::~SoEngineList()
{
}

/*!
  Append \a ptr to the list.

  \sa SoBaseList::append()
*/
void
SoEngineList::append(SoEngine * const ptr)
{
  SoBaseList::append((SoBase *)ptr);
}

/*!
  Return engine pointer at index \a i.

  \sa SoBaseList::operator[]()
*/
SoEngine *
SoEngineList::operator[](const int i) const
{
  return (SoEngine *)SoBaseList::operator[](i);
}

/*!
  Copy contents of list \a el to this list.

  \sa SoBaseList::operator=()
*/
SoEngineList &
SoEngineList::operator=(const SoEngineList & el)
{
  this->copy(el);
  return *this;
}
