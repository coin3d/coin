/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoMFEnum SoMFEnum.h Inventor/fields/SoMFEnum.h
  \brief The SoMFEnum class is a container for a set of enumerated values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store values constrained to be from an enumerated set.

  A field of this type stores its values to file as the symbolic
  names, rather than the actual integer values.

  \sa SoSFEnum

*/

#include <Inventor/fields/SoMFEnum.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>

/*!
  \var int SoMFEnum::numEnums
  Number of enumeration mappings.
*/
/*!
  \var SbName * SoMFEnum::enumNames
  Array of enumeration names. Maps 1-to-1 with the enumValues.
*/
/*!
  \var int * SoMFEnum::enumValues
  Array of enumeration values. Maps 1-to-1 with the enumNames.
*/
/*!
  \var SbBool SoMFEnum::legalValuesSet
  Is \c TRUE if a set of enum name-to-value mappings has been set.
*/



SO_MFIELD_REQUIRED_SOURCE(SoMFEnum);
SO_MFIELD_MALLOC_SOURCE(SoMFEnum, int);
SO_MFIELD_VALUE_SOURCE(SoMFEnum, int, int);


// (Declarations hidden in SO_[S|M]FIELD_HEADER macro in header file,
// so don't use Doxygen commenting.)
#ifndef DOXYGEN_SKIP_THIS

/* Constructor. */
SoMFEnum::SoMFEnum(void)
{
  this->values = NULL;
  this->legalValuesSet = FALSE;
  this->numEnums = 0;
  this->enumValues = NULL;
  this->enumNames = NULL;
}

/* Destructor. */
SoMFEnum::~SoMFEnum()
{
  this->deleteAllValues();
  delete[] this->enumValues;
  delete[] this->enumNames;
}

#endif // DOXYGEN_SKIP_THIS


// Override from parent class.
void
SoMFEnum::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFEnum);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFEnum::read1Value(SoInput * in, int idx)
{
  // FIXME: in this case, perhaps we should rather accept numeric
  // values instead of demanding mnemonics? 20020630 mortene.
  if (!this->legalValuesSet) {
    SbName name;
    SoFieldContainer * container = this->getContainer();
    SbBool fname = container && container->getFieldName(this, name);
    SoReadError::post(in,
                      "no mappings available for SoMFEnum field %s",
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

  this->set1Value(idx, val);
  return TRUE;
}

void
SoMFEnum::write1Value(SoOutput * out, int idx) const
{
  int val = (*this)[idx];
  for (int i = 0; i < this->numEnums; i++) {
    if (this->enumValues[i] == val) {
      out->write((char *)this->enumNames[i].getString());
      return;
    }
  }

#if COIN_DEBUG
  SoDebugError::post("SoMFEnum::writeValue", "Illegal value (%d) in field", val);
#endif // COIN_DEBUG
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set this field to contain a single value by specifying an
  enumeration string.
*/
void
SoMFEnum::setValue(const SbName name)
{
  int val;
  if (this->findEnumValue(name, val)) {
    this->setValue(val);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFEnum::setValue",
                       "Unknown enum '%s'", name.getString());
  }
#endif // COIN_DEBUG
}

/*!
  Set the value at \a idx to the enumeration value represented
  by \a name.
*/
void
SoMFEnum::set1Value(const int idx, const SbName name)
{
  int val;
  if(this->findEnumValue(name, val)) {
    this->set1Value(idx, val);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFEnum::setValue",
                       "Unknown enum '%s'", name.getString());
  }
#endif // COIN_DEBUG
}

/*!
  Makes a set of \a num enumeration \a names map to \a vals.
*/
void
SoMFEnum::setEnums(const int num, const int * const vals,
                    const SbName * const names)
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
SoMFEnum::findEnumValue(const SbName & name, int & val)
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
SoMFEnum::findEnumName(int value, const SbName * & name) const
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

/*!
  Returns the number of enum names the SoSFEnum object understands.

  \since 2001-01-22
*/
int
SoMFEnum::getNumEnums(void) const
{
  return this->numEnums;
}

/*!
  Returns the value of the Nth enum this SoSFEnum object understands,
  and mutates \a name to contain the Nth enum's name.

  \since 2001-01-22
*/
int
SoMFEnum::getEnum(const int idx, SbName & name) const
{
  if ( COIN_DEBUG && (idx < 0 || idx >= this->numEnums) ) {
    SoDebugError::post("SoSFEnum::getEnum", "idx (%d) out of range", idx);
    return -1;
  }
  name = this->enumNames[idx];
  return this->enumValues[idx];
}

