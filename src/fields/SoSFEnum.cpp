/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>


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


// (Declarations hidden in SO_[S|M]FIELD_HEADER macro in header file,
// so don't use Doxygen commenting.)
#ifndef DOXYGEN_SKIP_THIS

/* Constructor. */
SoSFEnum::SoSFEnum(void)
{
  this->enumValues = NULL;
  this->enumNames  = NULL;
  this->numEnums = 0;
  this->legalValuesSet = FALSE;
}

/* Destructor. */
SoSFEnum::~SoSFEnum()
{
  delete[] this->enumValues;
  delete[] this->enumNames;
}

/* Copy enumeration mappings and \a field value. */
const SoSFEnum &
SoSFEnum::operator=(const SoSFEnum & field)
{
  this->setEnums(field.numEnums, field.enumValues, field.enumNames);
  this->setValue(field.getValue());
  return *this;
}

#endif // DOXYGEN_SKIP_THIS


// Override from parent class.
void
SoSFEnum::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFEnum);
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
  // FIXME: in this case, perhaps we should rather accept numeric
  // values instead of demanding mnemonics? 20020630 mortene.
  if (!this->legalValuesSet) {
    SbName name;
    SoFieldContainer * container = this->getContainer();
    SbBool fname = container && container->getFieldName(this, name);
    SoReadError::post(in,
                      "no mappings available for SoSFEnum field %s",
                      fname ? name.getString() : "");
    return FALSE;
  }

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

/*!
  Returns the number of enum names the SoSFEnum object understands.

  Note that this API method is not part of the original SGI Inventor
  2.1 API, and is an extension specific to Coin.

  \since 2001-01-22
*/
int
SoSFEnum::getNumEnums(void) const
{
  return this->numEnums;
}

/*!
  Returns the value of the Nth enum this SoSFEnum object understands,
  and mutates \a name to contain the Nth enum's name.

  Note that this API method is not part of the original SGI Inventor
  2.1 API, and is an extension specific to Coin.

  \since 2001-01-22
*/
int
SoSFEnum::getEnum(const int idx, SbName & name) const
{
  if ( COIN_DEBUG && (idx < 0 || idx >= this->numEnums) ) {
    SoDebugError::post("SoSFEnum::getEnum", "idx (%d) out of range", idx);
    return -1;
  }
  name = this->enumNames[idx];
  return this->enumValues[idx];
}

