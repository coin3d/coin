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
  \class SoSFBool SoSFBool.h Inventor/fields/SoSFBool.h
  \brief The SoSFBool class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SbName.h>

#include <assert.h>

#if !defined(COIN_EXCLUDE_SOSFFLOAT)
#include <Inventor/fields/SoSFFloat.h>
#endif // !COIN_EXCLUDE_SOSFFLOAT
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
#if !defined(COIN_EXCLUDE_SOMFBOOL)
#include <Inventor/fields/SoMFBool.h>
#endif // !COIN_EXCLUDE_SOMFBOOL

// *************************************************************************

//$ BEGIN TEMPLATE SField( SoSFBool, const SbBool )

SoType SoSFBool::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFBool::getTypeId(void) const
{
  return SoSFBool::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFBool class.

  \sa getTypeId(), SoType
 */
SoType
SoSFBool::getClassTypeId(void)
{
  return SoSFBool::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFBool class.
*/
void *
SoSFBool::createInstance(void)
{
  return new SoSFBool;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFBool::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFBool::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFBool &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFBool::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFBool &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFBool &
SoSFBool::operator = (const SoSFBool & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFBool::SoSFBool(void)
{
}

/*!
  Destructor.
*/
SoSFBool::~SoSFBool()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFBool::setValue(const SbBool value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFBool::operator == (const SoSFBool & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField


/*!
  Does initialization common for all objects of the
  SoSFBool class. This includes setting up the
  type system, among other things.
*/
void
SoSFBool::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFBool )
  // Make sure we only initialize once.
  assert(SoSFBool::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFBool::classTypeId =
    SoType::createType(SoSFBool::getClassTypeId(),
                       "SFBool", &SoSFBool::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFBool::cleanClass(void)
{
}

SbBool
SoSFBool::readValue(SoInput * in)
{
  // FIXME: read binary format. 19990621 mortene.

  // accept 0 or 1
  if (in->read(value)) {
    if (value != 0 && value != 1) {
      SoReadError::post(in, "Illegal value for SoSFBool: %d "
			"(must be 0 or 1)", value);
      return FALSE;
    }
    return TRUE;
  }

  // read TRUE/FALSE keyword
  SbName n;
  if (! in->read(n, TRUE))
    return FALSE;
    
  if (n == "TRUE") {
    value = TRUE;
    return TRUE;
  }

  if (n == "FALSE") {
    value = FALSE;
    return TRUE;
  }

  SoReadError::post(in,
		    "Unknown value (\"%s\") for SoSFBool (must be TRUE or "
		    "FALSE)", n.getString());
  return FALSE;
}

void
SoSFBool::writeValue(SoOutput * out) const
{
  if (out->isBinary()) out->write((unsigned int) this->getValue() ? 1 : 0);
  else out->write(this->getValue() ? "TRUE" : "FALSE");
}

void
SoSFBool::convertTo(SoField * dest) const
{
  if (0) ;
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
  else if (dest->getTypeId()==SoSFFloat::getClassTypeId()) {
    ((SoSFFloat *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFFLOAT
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
    ((SoSFString *)dest)->setValue(this->getValue()?"TRUE":"FALSE");
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFBOOL)
  else if (dest->getTypeId()==SoMFBool::getClassTypeId()) {
    ((SoMFBool *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFBOOL
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFBool::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
