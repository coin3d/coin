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
  \brief The SoSFEnum class is a container for an enum value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store one particular value out of an enumerated set.

  A field of this type stores its value to file as the symbolic
  name, rather than the actual integer value.

  \sa SoMFEnum

*/

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  \var int SoSFEnum::numEnums
  Number of enumeration mappings.
*/
/*!
  \var SbName * SoSFEnum::enumNames
  Array of enumeration names. Maps 1-to-1 with the enumValues.
*/
/*!
  \var int * SoSFEnum::enumValues
  Array of enumeration values. Maps 1-to-1 with the enumNames.
*/
/*!
  \var SbBool SoSFEnum::legalValuesSet
  Is \c TRUE if a set of enum name-to-value mappings has been set.
*/



PRIVATE_TYPEID_SOURCE(SoSFEnum);
PRIVATE_EQUALITY_SOURCE(SoSFEnum);


/*!
  Copy enumeration mappings and \a field value.
*/
const SoSFEnum &
SoSFEnum::operator=(const SoSFEnum & field)
{
  this->setEnums(field.numEnums, field.enumValues, field.enumNames);
  this->setValue(field.getValue());
  return *this;
}

// Override from parent class.
void
SoSFEnum::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFEnum);
}

/*!
  Constructor.
*/
SoSFEnum::SoSFEnum(void)
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
  Makes a set of \a num enumeration \a names map to integer values,
  given by \a vals.
*/
void
SoSFEnum::setEnums(const int num, const int * vals, const SbName * names)
{
  delete[] this->enumValues;
  delete[] this->enumNames;

  this->enumValues = new int[num];
  this->enumNames = new SbName[num];
  this->numEnums = num;
  this->legalValuesSet = TRUE;

  for (int i = 0; i < this->numEnums; i++) {
    this->enumValues[i] = vals[i];
    this->enumNames[i] = names[i];
  }
}

/*!
  Return in \a val the enumeration value which matches the given
  enumeration \a name.

  Returns \c TRUE if \a name is a valid enumeration string, otherwise
  \c FALSE.
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
  Set the enumeration \a name which matches the given enumeration
  value.

  Returns \c TRUE if \a value is a valid enumeration value, otherwise
  \c FALSE.
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

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFEnum::readValue(SoInput * in)
{
  assert(this->legalValuesSet);

  SbName n;
  // Read mnemonic value as a character string identifier
  if (!in->read(n, TRUE)) {
    SoReadError::post(in, "Couldn't read enumeration name");
    return FALSE;
  }

  int val;
  if (!this->findEnumValue(n, val)) {
    SoReadError::post(in, "Unknown enumeration value \"%s\"", n.getString());
    return FALSE;
  }

  this->setValue(val);
  return TRUE;
}

void
SoSFEnum::writeValue(SoOutput * out) const
{
  int val = this->getValue();
  for (int i = 0; i < this->numEnums; i++) {
    if (this->enumValues[i] == val) {
      out->write((char *)this->enumNames[i].getString());
      return;
    }
  }

#if COIN_DEBUG
  SoDebugError::post("SoSFEnum::writeValue", "Illegal value (%d) in field", val);
#endif // COIN_DEBUG
}

/*!
  Compare this field with \a f and return \c TRUE if they are
  equal.
*/
int
SoSFEnum::operator==(const SoSFEnum & f) const
{
  // Check for value mismatch first.
  if (this->getValue() != f.getValue()) return FALSE;

  // Check that we have the same enumeration mappings.
  if (this->numEnums != f.numEnums) return FALSE;
  for (int i = 0; i < this->numEnums; i++) {
    if (this->enumValues[i] != f.enumValues[i]) return FALSE;
    if (this->enumNames[i] != f.enumNames[i]) return FALSE;
  }

  return TRUE;
}

/*!
  Set the value of this field by specifying an enumeration integer value.
*/
void
SoSFEnum::setValue(int newvalue)
{
  this->value = newvalue;
  this->valueChanged();
}

#endif // DOXYGEN_SKIP_THIS

/*!
  Set the value of this field by specifying an enumeration string value.
*/
void
SoSFEnum::setValue(const SbName name)
{
  int val;
  if (this->findEnumValue(name, val)) {
    this->setValue(val);
  }
  else {
#if COIN_DEBUG
    SoDebugError::post("SoSFEnum::setValue",
                       "Unknown enum '%s'", name.getString());
#endif // COIN_DEBUG
  }
}
