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
  \class SoDetail SoDetail.h Inventor/details/SoDetail.h
  \brief The SoDetail class is the superclass for all classes storing
  detailed information about particular shapes.
  \ingroup details

  Detail information about shapes is used in relation to picking
  actions in Coin.  They typically contain the relevant information
  about what particular part of the shape a pick ray intersected with.

  */

#include <Inventor/details/SoDetails.h>

SoType SoDetail::classTypeId;

/*!
  \fn SoDetail * SoDetail::copy(void) const
  Return a deep copy of ourself.

  \DANGEROUS_ALLOC_RETURN
*/

// Note: the following documentation for getTypeId() will also be
// visible for subclasses, so keep it general.  If you write any
// additional documentation for this method, check the other top-level
// classes with getTypeId() documentation to see if it is applicable
// to update those also (it probably is).
/*!
  \fn SoType SoDetail::getTypeId(void) const

  Returns the type identification of a detail derived from a class
  inheriting SoDetail.  This is used for run-time type checking and
  "downward" casting.

  Usage example:

  \code
  void fuhbear(SoDetail * detail)
  {
    if (detail->getTypeId() == SoFaceDetail::getClassTypeId()) {
      // safe downward cast, know the type
      SoFaceDetail * facedetail = (SoFaceDetail *)detail;
      /// [then something] ///
    }
    return; // ignore if not a SoFaceDetail
  }
  \endcode


  For application programmers wanting to extend the library with new
  detail classes: this method needs to be overridden in \e all
  subclasses. This is typically done as part of setting up the full
  type system for extension classes, which is usually accomplished by
  using the pre-defined macros available through
  Inventor/nodes/SoSubDetail.h: SO_DETAIL_SOURCE and
  SO_DETAIL_INIT_CLASS.
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
  return this->getTypeId().isDerivedFrom(type);
}

/*!
  Returns the type for this class.
*/
SoType
SoDetail::getClassTypeId(void)
{
  return SoDetail::classTypeId;
}

// Note: the following documentation for initClass() will also be
// visible for subclasses, so keep it general.
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

  (The initClass() method of user extension detail classes -- if any
  -- must be called explicitly by the application programmer in the
  application initialization code.)
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
