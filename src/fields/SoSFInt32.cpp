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
  \class SoSFInt32 SoSFInt32.h Inventor/fields/SoSFInt32.h
  \brief The SoSFInt32 class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOSFBOOL)
#include <Inventor/fields/SoSFBool.h>
#endif // !COIN_EXCLUDE_SOSFBOOL
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
#include <Inventor/fields/SoSFFloat.h>
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFUINT32)
#include <Inventor/fields/SoSFUInt32.h>
#endif // !COIN_EXCLUDE_SOSFUINT32
#if !defined(COIN_EXCLUDE_SOSFSHORT)
#include <Inventor/fields/SoSFShort.h>
#endif // !COIN_EXCLUDE_SOSFSHORT
#if !defined(COIN_EXCLUDE_SOSFUSHORT)
#include <Inventor/fields/SoSFUShort.h>
#endif // !COIN_EXCLUDE_SOSFUSHORT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFINT32)
#include <Inventor/fields/SoMFInt32.h>
#endif // !COIN_EXCLUDE_SOMFINT32

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32

// *************************************************************************

//$ BEGIN TEMPLATE SField( SoSFInt32, const int32_t )

SoType SoSFInt32::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFInt32::getTypeId(void) const
{
  return SoSFInt32::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFInt32 class.

  \sa getTypeId(), SoType
 */
SoType
SoSFInt32::getClassTypeId(void)
{
  return SoSFInt32::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFInt32 class.
*/
void *
SoSFInt32::createInstance(void)
{
  return new SoSFInt32;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFInt32::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFInt32::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFInt32 &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFInt32::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFInt32 &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFInt32 &
SoSFInt32::operator = (const SoSFInt32 & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFInt32::SoSFInt32(void)
{
  // Make sure we have initialized class.
  assert(SoSFInt32::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFInt32::~SoSFInt32()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFInt32::setValue(const int32_t value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFInt32::operator == (const SoSFInt32 & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField


/*!
  Does initialization common for all objects of the
  SoSFInt32 class. This includes setting up the
  type system, among other things.
*/
void
SoSFInt32::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFInt32 )
  // Make sure we only initialize once.
  assert(SoSFInt32::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFInt32::classTypeId =
    SoType::createType(SoSFInt32::getClassTypeId(),
                       "SFInt32", &SoSFInt32::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFInt32::cleanClass(void)
{
}

SbBool
SoSFInt32::readValue(SoInput * in)
{
  return in->read(value);
}

void
SoSFInt32::writeValue(SoOutput * out) const
{
  out->write(this->getValue());
}

void
SoSFInt32::convertTo(SoField * dest) const
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
#if !defined(COIN_EXCLUDE_SOMFINT32)
  else if (dest->getTypeId()==SoMFInt32::getClassTypeId()) {
    ((SoMFInt32 *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFINT32
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFInt32::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
