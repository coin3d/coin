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
  \class SoSField SoSField.h Inventor/fields/SoSField.h
  \brief The SoSField class is the base class for fields which contains
  only a single value.
  \ingroup fields

  All field types which should always contain only a single member
  value inherits this class. SoSField is an abstract class.

  \sa SoMField
*/


// Metadon doc:
/*¡
  Document the SO_SFIELD_HEADER, SO_SFIELD_SOURCE and
  SO_SFIELD_INIT_CLASS macros. Also write a note (with link to an
  online bookstore?) about the Inventor Toolmaker, if the application
  programmer should need to extend Coin with new single-value fields.
 */

#include <Inventor/fields/SoSField.h>

#include <Inventor/SbName.h>
#include <Inventor/SoOutput.h>
#include <Inventor/fields/SoSubField.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SoType SoSField::classTypeId = SoType::badType();

// Overridden from parent class.
SoType
SoSField::getClassTypeId(void)
{
  return SoSField::classTypeId;
}

/*!
  The SoSField constructor is protected, as this is an abstract
  class.
*/
SoSField::SoSField(void)
{
}

/*!
  The SoSField destructor is empty, and is only defined so we could
  make it virtual.
*/
SoSField::~SoSField()
{
}

// Overridden from parent class.
void
SoSField::initClass(void)
{
  PRIVATE_FIELD_INIT_CLASS(SoSField, "SField", inherited, NULL);
}

// If there's any single value fields without converter-functionality,
// calls to convertTo() will end up here.
void
SoSField::convertTo(SoField * dest) const
{
#if COIN_DEBUG
  SoDebugError::postWarning("SoSFBool::convertTo",
                            "Can't convert from %s to %s",
                            this->getTypeId().getName().getString(),
                            dest->getTypeId().getName().getString());
#endif // COIN_DEBUG
}
