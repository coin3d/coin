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
  \class SoDetail SoDetail.h Inventor/classes/SoDetail.h
  \brief The SoDetail class is yet to be documented.
  \ingroup details

  FIXME: write doc.
*/

#include <Inventor/details/SoDetails.h>

SoType SoDetail::classTypeId;

/*!
  \fn SoDetail * SoDetail::copy(void) const
  Return a copy of ourself.
*/


/*!
  Default constructor.
*/
SoDetail::SoDetail(void)
{
}

/*!
  Destructor.
*/
SoDetail::~SoDetail()
{
}

/*!
  Returns \c TRUE if \a type is derived from (or \e is) this class.
*/
SbBool
SoDetail::isOfType(const SoType type) const
{
  return type.isDerivedFrom(SoDetail::classTypeId);
}

/*!
  Returns the type for this class.
*/
SoType
SoDetail::getClassTypeId(void)
{
  return SoDetail::classTypeId;
}

/*!
  Initialize relevant common data for all instances, like the type
  system.
 */
void
SoDetail::initClass(void)
{
  SoDetail::classTypeId =
    SoType::createType(SoType::badType(), SbName("SoDetail"));
  SoDetail::initClasses();
}

/*!
  Call the initClass() methods of all built-in detail classes.
 */
void
SoDetail::initClasses(void)
{
  SoConeDetail::initClass();
  SoCubeDetail::initClass();
  SoCylinderDetail::initClass();
  SoFaceDetail::initClass();
  SoLineDetail::initClass();
  SoNodeKitDetail::initClass();
  SoPointDetail::initClass();
  SoTextDetail::initClass();
}
