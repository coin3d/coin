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
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/errors/SoReadError.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
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


// Override from parent class.
void
SoMFEnum::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFEnum);
}

/*!
  Constructor.
*/
SoMFEnum::SoMFEnum(void)
{
  this->values = NULL;
  this->legalValuesSet = FALSE;
  this->numEnums = 0;
  this->enumValues = NULL;
  this->enumNames = NULL;
}

/*!
  Destructor.
*/
SoMFEnum::~SoMFEnum()
{
  this->deleteAllValues();
  delete[] this->enumValues;
  delete[] this->enumNames;
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFEnum::read1Value(SoInput * in, int idx)
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


void
SoMFEnum::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFEnum::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFEnum *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFEnum::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
