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
  \class SoSFString SoSFString.h Inventor/fields/SoSFString.h
  \brief The SoSFString class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFString.h>
#if !defined(COIN_EXCLUDE_SOMFSTRING)
#include <Inventor/fields/SoMFString.h>
#endif // !COIN_EXCLUDE_SOMFSTRING
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE SField( SoSFString, const SbString & )

SoType SoSFString::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFString::getTypeId(void) const
{
  return SoSFString::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFString class.

  \sa getTypeId(), SoType
 */
SoType
SoSFString::getClassTypeId(void)
{
  return SoSFString::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFString class.
*/
void *
SoSFString::createInstance(void)
{
  return new SoSFString;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFString::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFString::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFString &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFString::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFString &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFString &
SoSFString::operator = (const SoSFString & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFString::SoSFString(void)
{
  // Make sure we have initialized class.
  assert(SoSFString::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFString::~SoSFString()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFString::setValue(const SbString & value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFString::operator == (const SoSFString & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

/*!
  Does initialization common for all objects of the
  SoSFString class. This includes setting up the
  type system, among other things.
*/
void
SoSFString::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFString )
  // Make sure we only initialize once.
  assert(SoSFString::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFString::classTypeId =
    SoType::createType(SoSFString::getClassTypeId(),
                       "SFString", &SoSFString::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFString::cleanClass(void)
{
}

SbBool
SoSFString::readValue(SoInput * in)
{
  return in->read(value);
}

void
SoSFString::writeValue(SoOutput * out) const
{
  out->write(this->getValue());
}

/*!
  FIXME: write function documentation
*/
void
SoSFString::setValue(const char * const string)
{
  this->setValue(SbString(string));
}

void
SoSFString::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOMFSTRING)
  else if (dest->getTypeId()==SoMFString::getClassTypeId()) {
    ((SoMFString *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFSTRING
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFString::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
