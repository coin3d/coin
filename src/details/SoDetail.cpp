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
