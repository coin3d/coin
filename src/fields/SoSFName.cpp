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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoSFName SoSFName.h Inventor/fields/SoSFName.h
  \brief The SoSFName class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFName.h>
#if !defined(COIN_EXCLUDE_SOMFNAME)
#include <Inventor/fields/SoMFName.h>
#endif // !COIN_EXCLUDE_SOMFNAME
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <strstream.h>

// *************************************************************************

//$ BEGIN TEMPLATE SField( SoSFName, const SbName )

SoType SoSFName::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFName::getTypeId(void) const
{
  return SoSFName::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFName class.

  \sa getTypeId(), SoType
 */
SoType
SoSFName::getClassTypeId(void)
{
  return SoSFName::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFName class.
*/
void *
SoSFName::createInstance(void)
{
  return new SoSFName;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFName::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFName::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFName &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFName::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFName &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFName &
SoSFName::operator = (const SoSFName & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFName::SoSFName(void)
{
  // Make sure we have initialized class.
  assert(SoSFName::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFName::~SoSFName()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFName::setValue(const SbName value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFName::operator == (const SoSFName & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoSFName class. This includes setting up the
  type system, among other things.
*/
void
SoSFName::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFName )
  // Make sure we only initialize once.
  assert(SoSFName::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFName::classTypeId =
    SoType::createType(SoSFName::getClassTypeId(),
                       "SFName", &SoSFName::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFName::cleanClass(void)
{
}

SbBool
SoSFName::readValue(SoInput * in)
{
  return in->read(value);
}

void
SoSFName::writeValue(SoOutput * out) const
{
  out->write(this->getValue());
}

/*!
  FIXME: write function documentation
*/
void
SoSFName::setValue(const char * const name)
{
  this->setValue(SbName(name));
}

void
SoSFName::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOMFNAME)
  else if (dest->getTypeId()==SoMFName::getClassTypeId()) {
    ((SoMFName *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFNAME
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    ostr << '"' << this->getValue().getString() << '"';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFName::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
