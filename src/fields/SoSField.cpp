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

  You use methods setValue() and getValue() to store or fetch the
  value of single-value fields. Example:

  \code
    SoSpotLight * lightnode = new SoSpotLight;
    lightnode->on.setValue(TRUE); // The "on" field of SoSpotLight is
                                  // a single value field of type SoSFBool.
    ...
    ...
    // Change lightswitch.
    if (lightnode->on.getValue() == FALSE)
      lightnode->on = TRUE; // We can use operator = instead of setValue().
    else
      lightnode->on = FALSE;
    ...
  \endcode

  When nodes, engines or other types of field containers are written
  to file, their single-value fields are written to file in this
  format:

  \code
    containerclass {
      fieldname value
      fieldname value
      ...
    }
  \endcode

  ..like this, for instance, a SpotLight node from a scene
  graph which will be default \e off when read back from file:

  \code
    SpotLight {
      on FALSE
    }
  \endcode

  \sa SoMField */


// Metadon doc:
/*¡
  Document the SO_SFIELD_HEADER, SO_SFIELD_SOURCE and
  SO_SFIELD_INIT_CLASS macros. Also write a note (with link to an
  online bookstore?) about the Inventor Toolmaker, if the application
  programmer should need to extend Coin with new single-value fields.
 */

#include <Inventor/fields/SoSField.h>

#include <Inventor/fields/SoSubField.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


// Don't set value explicitly to SoType::badType(), to avoid a bug in
// Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()).
SoType SoSField::classTypeId;

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
