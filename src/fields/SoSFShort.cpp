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
  \class SoSFShort SoSFShort.h Inventor/fields/SoSFShort.h
  \brief The SoSFShort class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFShort.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>

#if !defined(COIN_EXCLUDE_SOSFBOOL)
#include <Inventor/fields/SoSFBool.h>
#endif // !COIN_EXCLUDE_SOSFBOOL
#if !defined(COIN_EXCLUDE_SOSFINT32)
#include <Inventor/fields/SoSFInt32.h>
#endif // !COIN_EXCLUDE_SOSFINT32
#if !defined(COIN_EXCLUDE_SOSFUINT32)
#include <Inventor/fields/SoSFUInt32.h>
#endif // !COIN_EXCLUDE_SOSFUINT32
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
#include <Inventor/fields/SoSFFloat.h>
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFUSHORT)
#include <Inventor/fields/SoSFUShort.h>
#endif // !COIN_EXCLUDE_SOSFUSHORT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFSHORT)
#include <Inventor/fields/SoMFShort.h>
#endif // !COIN_EXCLUDE_SOMFSHORT

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

//$ BEGIN TEMPLATE SField(SoSFShort, const short)

SoType SoSFShort::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFShort::getTypeId(void) const
{
  return SoSFShort::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFShort class.

  \sa getTypeId(), SoType
 */
SoType
SoSFShort::getClassTypeId(void)
{
  return SoSFShort::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFShort class.
*/
void *
SoSFShort::createInstance(void)
{
  return new SoSFShort;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFShort::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFShort::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFShort &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFShort::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFShort &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFShort &
SoSFShort::operator = (const SoSFShort & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFShort::SoSFShort(void)
{
  // Make sure we have initialized class.
  assert(SoSFShort::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFShort::~SoSFShort()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFShort::setValue(const short value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFShort::operator == (const SoSFShort & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

/*!
  Does initialization common for all objects of the
  SoSFShort class. This includes setting up the
  type system, among other things.
*/
void
SoSFShort::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(SFShort)
  // Make sure we only initialize once.
  assert(SoSFShort::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFShort::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SFShort", &SoSFShort::createInstance);
//$ END TEMPLATE FieldInitClass
}

SbBool
SoSFShort::readValue(SoInput * in)
{
  return in->read(value);
}

void
SoSFShort::writeValue(SoOutput * out) const
{
  out->write(this->getValue()); // using getValue() to evaluate
}

void
SoSFShort::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFBOOL)
  else if (dest->getTypeId()==SoSFBool::getClassTypeId()) {
    ((SoSFBool *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFBOOL
#if !defined(COIN_EXCLUDE_SOSFINT32)
  else if (dest->getTypeId()==SoSFInt32::getClassTypeId()) {
    ((SoSFInt32 *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFINT32
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
  else if (dest->getTypeId()==SoSFFloat::getClassTypeId()) {
    ((SoSFFloat *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFUINT32)
  else if (dest->getTypeId()==SoSFUInt32::getClassTypeId()) {
    ((SoSFUInt32 *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFUINT32
#if !defined(COIN_EXCLUDE_SOSFUSHORT)
  else if (dest->getTypeId()==SoSFUShort::getClassTypeId()) {
    ((SoSFUShort *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFUSHORT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    ostr << this->getValue() << '\0';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFSHORT)
  else if (dest->getTypeId()==SoMFShort::getClassTypeId()) {
    ((SoMFShort *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFSHORT
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFShort::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
