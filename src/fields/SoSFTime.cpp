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
  \class SoSFTime SoSFTime.h Inventor/fields/SoSFTime.h
  \brief The SoSFTime class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFTime.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOSFFLOAT)
#include <Inventor/fields/SoSFFloat.h>
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFTIME)
#include <Inventor/fields/SoMFTime.h>
#endif // !COIN_EXCLUDE_SOMFTIME

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <strstream.h>

// *************************************************************************

//$ BEGIN TEMPLATE SField( SoSFTime, const SbTime & )

SoType SoSFTime::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFTime::getTypeId(void) const
{
  return SoSFTime::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFTime class.

  \sa getTypeId(), SoType
 */
SoType
SoSFTime::getClassTypeId(void)
{
  return SoSFTime::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFTime class.
*/
void *
SoSFTime::createInstance(void)
{
  return new SoSFTime;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFTime::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFTime::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFTime &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFTime::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFTime &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFTime &
SoSFTime::operator = (const SoSFTime & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFTime::SoSFTime(void)
{
  // Make sure we have initialized class.
  assert(SoSFTime::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFTime::~SoSFTime()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFTime::setValue(const SbTime & value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFTime::operator == (const SoSFTime & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoSFTime class. This includes setting up the
  type system, among other things.
*/
void
SoSFTime::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFTime )
  // Make sure we only initialize once.
  assert(SoSFTime::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFTime::classTypeId =
    SoType::createType(SoSFTime::getClassTypeId(),
                       "SFTime", &SoSFTime::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFTime::cleanClass(void)
{
}

SbBool
SoSFTime::readValue(SoInput * in)
{
  double val;
  if (!in->read(val)) return FALSE;
  this->value.setValue(val);
  return TRUE;
}

void
SoSFTime::writeValue(SoOutput * out) const
{
  out->write(this->getValue().getValue());
}

void
SoSFTime::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
  else if (dest->getTypeId()==SoSFFloat::getClassTypeId()) {
    ((SoSFFloat *)dest)->setValue(this->getValue().getValue());
  }
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    if (this->getValue().getValue()>31500000.0)
      ((SoSFString *)dest)->setValue(this->getValue().formatDate());
    else
      ((SoSFString *)dest)->setValue(this->getValue().format());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFTIME)
  else if (dest->getTypeId()==SoMFTime::getClassTypeId()) {
    ((SoMFTime *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFTIME
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFTime::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
