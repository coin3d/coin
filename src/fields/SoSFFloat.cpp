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
  \class SoSFFloat SoSFFloat.h Inventor/fields/SoSFFloat.h
  \brief The SoSFFloat class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFFloat.h>
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
#if !defined(COIN_EXCLUDE_SOSFSHORT)
#include <Inventor/fields/SoSFShort.h>
#endif // !COIN_EXCLUDE_SOSFSHORT
#if !defined(COIN_EXCLUDE_SOSFUSHORT)
#include <Inventor/fields/SoSFUShort.h>
#endif // !COIN_EXCLUDE_SOSFUSHORT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFFLOAT)
#include <Inventor/fields/SoMFFloat.h>
#endif // !COIN_EXCLUDE_SOMFFLOAT
#if !defined(COIN_EXCLUDE_SOSFTIME)
#include <Inventor/fields/SoSFTime.h>
#endif // !COIN_EXCLUDE_SOSFTIME

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <strstream.h>

// *************************************************************************

//$ BEGIN TEMPLATE SField(SoSFFloat, const float)

SoType SoSFFloat::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFFloat::getTypeId(void) const
{
  return SoSFFloat::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFFloat class.

  \sa getTypeId(), SoType
 */
SoType
SoSFFloat::getClassTypeId(void)
{
  return SoSFFloat::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFFloat class.
*/
void *
SoSFFloat::createInstance(void)
{
  return new SoSFFloat;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFFloat::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFFloat::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFFloat &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFFloat::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFFloat &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFFloat &
SoSFFloat::operator = (const SoSFFloat & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFFloat::SoSFFloat(void)
{
}

/*!
  Destructor.
*/
SoSFFloat::~SoSFFloat()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFFloat::setValue(const float value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFFloat::operator == (const SoSFFloat & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

/*!
  Does initialization common for all objects of the
  SoSFFloat class. This includes setting up the
  type system, among other things.
*/
void
SoSFFloat::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(SFFloat)
  // Make sure we only initialize once.
  assert(SoSFFloat::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFFloat::classTypeId =
    SoType::createType(SoSFFloat::getClassTypeId(),
                       "SFFloat", &SoSFFloat::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFFloat::cleanClass(void)
{
}

SbBool
SoSFFloat::readValue(SoInput * in)
{
  return in->read(value);
}

void
SoSFFloat::writeValue(SoOutput * out) const
{
  out->write(this->value);
}

void
SoSFFloat::convertTo(SoField * dest) const
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
#if !defined(COIN_EXCLUDE_SOMFFLOAT)
  else if (dest->getTypeId()==SoMFFloat::getClassTypeId()) {
    ((SoMFFloat *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFFLOAT
#if !defined(COIN_EXCLUDE_SOSFTIME)
  else if (dest->getTypeId()==SoSFTime::getClassTypeId()) {
    ((SoSFTime *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFTIME
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFFloat::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
