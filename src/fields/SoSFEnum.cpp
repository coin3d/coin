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
  \class SoSFEnum SoSFEnum.h Inventor/fields/SoSFEnum.h
  \brief The SoSFEnum class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <assert.h>

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOMFENUM)
#include <Inventor/fields/SoMFEnum.h>
#endif // !COIN_EXCLUDE_SOMFENUM

/*!
  \var int SoSFEnum::numEnums
  FIXME: write var doc
*/
/*!
  \var int * SoSFEnum::enumValues
  FIXME: write var doc
*/
/*!
  \var SbName * SoSFEnum::enumNames
  FIXME: write var doc
*/
/*!
  \var SbBool SoSFEnum::legalValuesSet
  FIXME: write var doc
*/

// *************************************************************************

//$ BEGIN TEMPLATE FieldId( SoSFEnum )

SoType SoSFEnum::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFEnum::getTypeId(void) const
{
  return SoSFEnum::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFEnum class.

  \sa getTypeId(), SoType
 */
SoType
SoSFEnum::getClassTypeId(void)
{
  return SoSFEnum::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFEnum class.
*/
void *
SoSFEnum::createInstance(void)
{
  return new SoSFEnum;
}
//$ END TEMPLATE FieldId
//$ BEGIN TEMPLATE FieldEqSame( SoSFEnum )
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFEnum::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFEnum::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFEnum &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFEnum::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFEnum &) field);
}
//$ END TEMPLATE FieldEqSame

// *************************************************************************

/*!
  FIXME: write function documentation
*/
const SoSFEnum &
SoSFEnum::operator = (const SoSFEnum & field)
{
  this->setEnums(field.numEnums, field.enumValues, field.enumNames);
  this->setValue(field.getValue());
  return *this;
}

/*!
  Does initialization common for all objects of the
  SoSFEnum class. This includes setting up the
  type system, among other things.
*/
void
SoSFEnum::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFEnum )
  // Make sure we only initialize once.
  assert(SoSFEnum::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFEnum::classTypeId =
    SoType::createType(SoSFEnum::getClassTypeId(),
                       "SFEnum", &SoSFEnum::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFEnum::cleanClass(void)
{
}

/*!
  Constructor.
*/
SoSFEnum::SoSFEnum()
{
  this->enumValues = NULL;
  this->enumNames  = NULL;
  this->numEnums = 0;
  this->legalValuesSet = FALSE;
}

/*!
  Destructor.
*/
SoSFEnum::~SoSFEnum()
{
  delete[] this->enumValues;
  delete[] this->enumNames;
}

/*!
  FIXME: write function documentation
*/
void 
SoSFEnum::setEnums(const int num, const int * vals, const SbName * names)
{
  int * newvals = new int[num];
  SbName * newnames = new SbName[num];

  delete[] this->enumValues;
  delete[] this->enumNames;

  this->enumValues = newvals;
  this->enumNames = newnames;
  this->numEnums = num;

  for (int i = 0; i < this->numEnums; i++) {
    this->enumValues[i] = vals[i];
    this->enumNames[i] = names[i];
  }
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFEnum::findEnumValue(const SbName & name, int & val)
{
  // Look through names table for one that matches
  for (int i = 0; i < numEnums; i++) {
    if (name == enumNames[i]) {
      val = enumValues[i];
      return TRUE;
    }
  }
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFEnum::findEnumName(int value, const SbName * & name) const
{
  // Look through values table for one that matches
  for (int i = 0; i < numEnums; i++) {
    if (value == enumValues[i]) {
      name = &(enumNames[i]);
      return TRUE;
    }
  }
  return FALSE;
}

SbBool
SoSFEnum::readValue(SoInput * in)
{
  assert(!in->isBinary() && "FIXME: not implemented");

  assert(this->enumValues != NULL);

  SbName n;
  // Read mnemonic value as a character string identifier
  if (! in->read(n, TRUE))
    return FALSE;

  if (this->findEnumValue(n, this->value))
    return TRUE;

    // Not found? Too bad
  SoReadError::post(in, "Unknown SoSFEnum enumeration value \"%s\"",
		    n.getString());
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFEnum::setValue(int newValue)
{
  this->value = newValue;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoSFEnum::setValue(const SbName name)
{
  int val;
  SbBool result = this->findEnumValue(name, val);
  if(result) {
    this->setValue(val);
  }
  else {
    // FIXME: SoDebugError? 19980913 mortene.
  }
}

/*!
  FIXME: write function documentation
*/
int
SoSFEnum::operator ==(const SoSFEnum & f) const
{
  // FIXME: should check enum types aswell? 19980913 mortene.

  if (this->value != f.value)
    return FALSE;
  return TRUE;
}

void
SoSFEnum::writeValue(SoOutput * out) const
{
  assert(!out->isBinary() && "FIXME: not implemented");

  if (this->isDefault()) {
    // FIXME: what happens here on binary write? 19980913 mortene.
    out->write("DEFAULT");
  }
  else {
    SbBool written = FALSE;

    for (int i = 0; i < this->numEnums; i++) {
      if (this->enumValues[i] == this->value) {
	// FIXME: what happens here on binary write? 19980913 mortene.
	out->write((char *) this->enumNames[i].getString());
	written = TRUE;
	break;
      }
    }

#if COIN_DEBUG
    if(!written)
      SoDebugError::post("SoSFEnum::writeValue",
			 "Illegal value (%d) in field",
			 this->value);
#endif // COIN_DEBUG
  }
}

void
SoSFEnum::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOMFENUM)
  else if (dest->getTypeId()==SoMFEnum::getClassTypeId()) {
    ((SoMFEnum *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFENUM
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFEnum::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
