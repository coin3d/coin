/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#include <Inventor/fields/SoMFEnum.h>

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



PRIVATE_TYPEID_SOURCE(SoSFEnum);
PRIVATE_EQUALITY_SOURCE(SoSFEnum);


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
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFEnum);
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
  Return in \a val the enumeration value which matches the given
  enumeration string.

  Returns \a TRUE if \a name is a valid enumeration string, otherwise
  \a FALSE.
*/
SbBool
SoSFEnum::findEnumValue(const SbName & name, int & val)
{
  // Look through names table for one that matches
  for (int i = 0; i < this->numEnums; i++) {
    if (name == this->enumNames[i]) {
      val = this->enumValues[i];
      return TRUE;
    }
  }
  return FALSE;
}

/*!
  Return the enumeration string which matches the given enumeration value.

  Returns \a TRUE if \a value is a valid enumeration value, otherwise
  \a FALSE.
*/
SbBool
SoSFEnum::findEnumName(int value, const SbName *& name) const
{
  // Look through values table for one that matches
  for (int i = 0; i < this->numEnums; i++) {
    if (value == this->enumValues[i]) {
      name = &(this->enumNames[i]);
      return TRUE;
    }
  }
  return FALSE;
}

SbBool
SoSFEnum::readValue(SoInput * in)
{
  assert(this->enumValues != NULL);

  SbName n;
  // Read mnemonic value as a character string identifier
  if (!in->read(n, TRUE)) return FALSE;
  if (this->findEnumValue(n, this->value)) return TRUE;

  SoReadError::post(in, "Unknown SoSFEnum enumeration value \"%s\"",
                    n.getString());
  return FALSE;
}

/*!
  Set the value of this field by specifying an enumeration integer value.
*/
void
SoSFEnum::setValue(int newValue)
{
  this->value = newValue;
  this->valueChanged();
}

/*!
  Set the value of this field by specifying an enumeration string value.
*/
void
SoSFEnum::setValue(const SbName name)
{
  int val;
  SbBool result = this->findEnumValue(name, val);
  if (result) {
    this->setValue(val);
  }
  else {
#if COIN_DEBUG
    SoDebugError::postInfo("SoSFEnum::setValue",
                           "Unknown enum '%s'", name.getString());
#endif // COIN_DEBUG
  }
}

/*!
  FIXME: write function documentation
*/
int
SoSFEnum::operator ==(const SoSFEnum & f) const
{
  // FIXME: should check enum types aswell? 19980913 mortene.
  if (this->value != f.value) return FALSE;
  return TRUE;
}

void
SoSFEnum::writeValue(SoOutput * out) const
{
  SbBool written = FALSE;

  for (int i = 0; i < this->numEnums; i++) {
    if (this->enumValues[i] == this->value) {
      out->write((char *)this->enumNames[i].getString());
      written = TRUE;
      break;
    }
  }

#if COIN_DEBUG
  // FIXME: this will in the common case screw up the output file so
  // it can't be re-read. 19990713 mortene.
  if (!written)
    SoDebugError::post("SoSFEnum::writeValue", "Illegal value (%d) in field",
                       this->value);
#endif // COIN_DEBUG
}

void
SoSFEnum::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoMFEnum::getClassTypeId()) {
    ((SoMFEnum *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFEnum::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
