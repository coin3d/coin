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
  \class SoMFTime SoMFTime.h Inventor/fields/SoMFTime.h
  \brief The SoMFTime class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFTime.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE MField(SoMFTime, SbTime, const SbTime &)

SoType SoMFTime::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFTime::getTypeId(void) const
{
  return SoMFTime::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFTime class.

  \sa getTypeId(), SoType
 */
SoType
SoMFTime::getClassTypeId(void)
{
  return SoMFTime::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFTime class.
*/
void *
SoMFTime::createInstance(void)
{
  return new SoMFTime;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFTime::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFTime::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFTime &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFTime::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFTime &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFTime &
SoMFTime::operator = (const SoMFTime & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFTime::SoMFTime(void)
{
  // Make sure we have initialized class.
  assert(SoMFTime::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFTime::~SoMFTime(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFTime::fieldSizeof(void) const
{
  return sizeof(SbTime);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFTime::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFTime::setValuesPtr(void * ptr)
{
  this->values = (SbTime *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFTime::find(const SbTime & value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFTime::setValues(const int start, const int num,
                       const SbTime * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SbTime) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFTime::set1Value(const int idx, const SbTime & value)
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
SoMFTime::setValue(const SbTime & value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFTime::operator == (const SoMFTime & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SbTime * const lhs = this->getValues(0);
  const SbTime * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFTime::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFTime::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFTime class. This includes setting up the
  type system, among other things.
*/
void
SoMFTime::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(MFTime)
  // Make sure we only initialize once.
  assert(SoMFTime::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFTime::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "MFTime", &SoMFTime::createInstance);
//$ END TEMPLATE FieldInitClass
}

// *************************************************************************

SbBool
SoMFTime::read1Value(SoInput * in, int idx)
{
  SoSFTime sftime;
  SbBool result;
  if ((result = sftime.readValue(in))) this->set1Value(idx, sftime.getValue());
  return result;
}

void
SoMFTime::write1Value(SoOutput * out, int idx) const
{
  SoSFTime sftime;
  sftime.setValue((*this)[idx]);
  sftime.writeValue(out);
}

void
SoMFTime::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFTime::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFTime *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFTime::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
