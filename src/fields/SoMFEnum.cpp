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
  \class SoMFEnum SoMFEnum.h Inventor/fields/SoMFEnum.h
  \brief The SoMFEnum class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFEnum.h>
#include <Inventor/fields/SoSFEnum.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <assert.h>

/*!
  \var SbBool SoMFEnum::legalValuesSet
  FIXME: write doc
*/
/*!
  \var int SoMFEnum::numEnums
  FIXME: write doc
*/
/*!
  \var int * SoMFEnum::enumValues
  FIXME: write doc
*/
/*!
  \var SbName * SoMFEnum::enumNames
  FIXME: write doc
*/

// *************************************************************************

//$ BEGIN TEMPLATE MFieldRequired(SoMFEnum)

SoType SoMFEnum::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFEnum::getTypeId(void) const
{
  return SoMFEnum::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFEnum class.

  \sa getTypeId(), SoType
 */
SoType
SoMFEnum::getClassTypeId(void)
{
  return SoMFEnum::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFEnum class.
*/
void *
SoMFEnum::createInstance(void)
{
  return new SoMFEnum;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFEnum::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFEnum::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFEnum &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFEnum::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFEnum &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFEnum &
SoMFEnum::operator = (const SoMFEnum & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}
//$ END TEMPLATE MFieldRequired
//$ BEGIN TEMPLATE MFieldValue(SoMFEnum, int, int)

/*!
  FIXME: write function documentation
*/
int
SoMFEnum::fieldSizeof(void) const
{
  return sizeof(int);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFEnum::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFEnum::setValuesPtr(void * ptr)
{
  this->values = (int *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFEnum::find(int value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFEnum::setValues(const int start, const int num,
                       const int * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (int) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFEnum::set1Value(const int idx, int value)
{
  if (idx+1 > this->maxNum) this->allocValues(idx+1);
  else if (idx+1 > this->num) this->num = idx+1;
  this->values[idx] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFEnum::setValue(int value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFEnum::operator == (const SoMFEnum & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const int * const lhs = this->getValues(0);
  const int * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFEnum::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFEnum::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MFieldValue

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFEnum class. This includes setting up the
  type system, among other things.
*/
void
SoMFEnum::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(MFEnum)
  // Make sure we only initialize once.
  assert(SoMFEnum::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFEnum::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "MFEnum", &SoMFEnum::createInstance);
//$ END TEMPLATE FieldInitClass
}

// *************************************************************************

/*!
  Constructor.
*/
SoMFEnum::SoMFEnum(void)
{
  this->values = NULL;
  this->legalValuesSet = TRUE;
  this->numEnums = 0;
  this->enumValues = NULL;
  this->enumNames = NULL;
}

/*!
  Destructor.
*/
SoMFEnum::~SoMFEnum(void)
{
  this->deleteAllValues();
}

SbBool
SoMFEnum::read1Value(SoInput * in, int idx)
{
  SoSFEnum sfenum;
  sfenum.setEnums(this->numEnums, this->enumValues, this->enumNames);
  SbBool result;
  if ((result = sfenum.readValue(in)))
    this->set1Value(idx, sfenum.getValue());
  return result;
}

void
SoMFEnum::write1Value(SoOutput * out, int idx) const
{
  SoSFEnum sfenum;
  sfenum.setEnums(this->numEnums, this->enumValues, this->enumNames);
  sfenum.setValue((*this)[idx]);
  sfenum.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFEnum::setValue(const SbName name)
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
void
SoMFEnum::set1Value(const int idx, const SbName name)
{
  int val;
  SbBool result = this->findEnumValue(name, val);
  if(result) {
    this->set1Value(idx, val);
  }
  else {
    // FIXME: SoDebugError? 19980913 mortene.
  }
}

/*!
  FIXME: write function documentation
*/
void
SoMFEnum::setEnums(const int num, const int * const vals,
                    const SbName * const names)
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
SoMFEnum::findEnumValue(const SbName & name, int & val)
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
SoMFEnum::findEnumName(int val, const SbName * & name) const
{
  // Look through values table for one that matches
  for (int i = 0; i < numEnums; i++) {
    if (val == enumValues[i]) {
      name = &(enumNames[i]);
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
