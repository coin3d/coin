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
  \class SoSFUInt32 SoSFUInt32.h Inventor/fields/SoSFUInt32.h
  \brief The SoSFUInt32 class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFUInt32.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOSFBOOL)
#include <Inventor/fields/SoSFBool.h>
#endif // !COIN_EXCLUDE_SOSFBOOL
#if !defined(COIN_EXCLUDE_SOSFINT32)
#include <Inventor/fields/SoSFInt32.h>
#endif // !COIN_EXCLUDE_SOSFINT32
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
#include <Inventor/fields/SoSFFloat.h>
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFSHORT)
#include <Inventor/fields/SoSFShort.h>
#endif // !COIN_EXCLUDE_SOSFSHORT
#if !defined(COIN_EXCLUDE_SOSFUSHORT)
#include <Inventor/fields/SoSFUShort.h>
#endif // !COIN_EXCLUDE_SOSFUSHORT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFUINT32)
#include <Inventor/fields/SoMFUInt32.h>
#endif // !COIN_EXCLUDE_SOMFUINT32

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32

// *************************************************************************

//$ BEGIN TEMPLATE SField(SoSFUInt32, const uint32_t)

SoType SoSFUInt32::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFUInt32::getTypeId(void) const
{
  return SoSFUInt32::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFUInt32 class.

  \sa getTypeId(), SoType
 */
SoType
SoSFUInt32::getClassTypeId(void)
{
  return SoSFUInt32::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFUInt32 class.
*/
void *
SoSFUInt32::createInstance(void)
{
  return new SoSFUInt32;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFUInt32::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFUInt32::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFUInt32 &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFUInt32::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFUInt32 &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFUInt32 &
SoSFUInt32::operator = (const SoSFUInt32 & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFUInt32::SoSFUInt32(void)
{
  // Make sure we have initialized class.
  assert(SoSFUInt32::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFUInt32::~SoSFUInt32()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFUInt32::setValue(const uint32_t value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFUInt32::operator == (const SoSFUInt32 & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoSFUInt32 class. This includes setting up the
  type system, among other things.
*/
void
SoSFUInt32::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(SFUInt32)
  // Make sure we only initialize once.
  assert(SoSFUInt32::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFUInt32::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SFUInt32", &SoSFUInt32::createInstance);
//$ END TEMPLATE FieldInitClass
}

SbBool
SoSFUInt32::readValue(SoInput * in)
{
  return in->read(value);
}

void
SoSFUInt32::writeValue(SoOutput * out) const
{
  out->write(this->getValue()); // using getValue() to evaluate
}

void
SoSFUInt32::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFBOOL)
  else if (dest->getTypeId()==SoSFBool::getClassTypeId()) {
    ((SoSFBool *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFBOOL
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
  else if (dest->getTypeId()==SoSFFloat::getClassTypeId()) {
    ((SoSFFloat *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFSHORT)
  else if (dest->getTypeId()==SoSFShort::getClassTypeId()) {
    ((SoSFShort *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFSHORT
#if !defined(COIN_EXCLUDE_SOSFINT32)
  else if (dest->getTypeId()==SoSFInt32::getClassTypeId()) {
    ((SoSFInt32 *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFINT32
#if !defined(COIN_EXCLUDE_SOSFUSHORT)
  else if (dest->getTypeId()==SoSFUShort::getClassTypeId()) {
    ((SoSFUShort *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFUSHORT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    if (this->getValue()>0) ostr.flags(ios::hex|ios::showbase);
    ostr << this->getValue() << '\0';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFUINT32)
  else if (dest->getTypeId()==SoMFUInt32::getClassTypeId()) {
    ((SoMFUInt32 *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFUINT32
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFUInt32::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
