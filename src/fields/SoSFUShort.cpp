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
  \class SoSFUShort SoSFUShort.h Inventor/fields/SoSFUShort.h
  \brief The SoSFUShort class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFUShort.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFUInt32.h>
#include <Inventor/fields/SoSFShort.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFUShort.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32

#include <iomanip.h>

// *************************************************************************

//$ BEGIN TEMPLATE SField(SoSFUShort, const unsigned short)

SoType SoSFUShort::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFUShort::getTypeId(void) const
{
  return SoSFUShort::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFUShort class.

  \sa getTypeId(), SoType
 */
SoType
SoSFUShort::getClassTypeId(void)
{
  return SoSFUShort::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFUShort class.
*/
void *
SoSFUShort::createInstance(void)
{
  return new SoSFUShort;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFUShort::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFUShort::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFUShort &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFUShort::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFUShort &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFUShort &
SoSFUShort::operator = (const SoSFUShort & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFUShort::SoSFUShort(void)
{
  // Make sure we have initialized class.
  assert(SoSFUShort::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFUShort::~SoSFUShort()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFUShort::setValue(const unsigned short value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFUShort::operator == (const SoSFUShort & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoSFUShort class. This includes setting up the
  type system, among other things.
*/
void
SoSFUShort::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(SFUShort)
  // Make sure we only initialize once.
  assert(SoSFUShort::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFUShort::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SFUShort", &SoSFUShort::createInstance);
//$ END TEMPLATE FieldInitClass
}

SbBool
SoSFUShort::readValue(SoInput * in)
{
  return in->read(value);
}

void
SoSFUShort::writeValue(SoOutput * out) const
{
  out->write(this->getValue()); // using getValue() to evaluate
}

void
SoSFUShort::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFBool::getClassTypeId()) {
    ((SoSFBool *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFInt32::getClassTypeId()) {
    ((SoSFInt32 *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFFloat::getClassTypeId()) {
    ((SoSFFloat *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFUInt32::getClassTypeId()) {
    ((SoSFUInt32 *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFShort::getClassTypeId()) {
    ((SoSFShort *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    if (this->getValue()>0) ostr.flags(ios::hex|ios::showbase);
    ostr << this->getValue() << '\0';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
  else if (dest->getTypeId()==SoMFUShort::getClassTypeId()) {
    ((SoMFUShort *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFUShort::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
