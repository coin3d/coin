/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoSField SoField.h Inventor/fields/SoField.h
  \brief The SoSField class is the abstract base class for fields which contains only a single value.
  \ingroup fields

  FIXME: write class doc

  \sa SoField, SoMField
*/

#include <Inventor/fields/SoSField.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <assert.h>


SoType SoSField::classTypeId = SoType::badType();

/*!
  Returns a unique type identifier for the SoSField class.

  \sa getTypeId(), SoType
 */
SoType
SoSField::getClassTypeId(void)
{
  return SoSField::classTypeId;
}

/*!
  Constructor.
*/
SoSField::SoSField(void)
{
}

/*!
  Destructor.
*/
SoSField::~SoSField()
{
}

/*!
  Must be called upon initialization of the library to set up
  the type system.
*/
void
SoSField::initClass(void)
{
  // Make sure we only initialize once.
  assert(SoSField::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSField::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "SField", NULL);
}

/*!
  FIXME: write function documentation
*/
void
SoSField::convertTo(SoField * /* dest */) const
{
  // Should never be called, all subclasses with conversion
  // functionality need to reimplement this method.
  assert(0);
}
