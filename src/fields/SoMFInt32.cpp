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
  \class SoMFInt32 SoMFInt32.h Inventor/fields/SoMFInt32.h
  \brief The SoMFInt32 class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFInt32.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/fields/SoSFString.h>

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32

// *************************************************************************

//$ BEGIN TEMPLATE MField(SoMFInt32, int32_t, int32_t)

SoType SoMFInt32::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFInt32::getTypeId(void) const
{
  return SoMFInt32::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFInt32 class.

  \sa getTypeId(), SoType
 */
SoType
SoMFInt32::getClassTypeId(void)
{
  return SoMFInt32::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFInt32 class.
*/
void *
SoMFInt32::createInstance(void)
{
  return new SoMFInt32;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFInt32::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFInt32::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFInt32 &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFInt32::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFInt32 &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFInt32 &
SoMFInt32::operator = (const SoMFInt32 & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFInt32::SoMFInt32(void)
{
  // Make sure we have initialized class.
  assert(SoMFInt32::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFInt32::~SoMFInt32(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFInt32::fieldSizeof(void) const
{
  return sizeof(int32_t);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFInt32::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFInt32::setValuesPtr(void * ptr)
{
  this->values = (int32_t *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFInt32::find(int32_t value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFInt32::setValues(const int start, const int num,
                       const int32_t * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (int32_t) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFInt32::set1Value(const int idx, int32_t value)
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
SoMFInt32::setValue(int32_t value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFInt32::operator == (const SoMFInt32 & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const int32_t * const lhs = this->getValues(0);
  const int32_t * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFInt32::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFInt32::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFInt32 class. This includes setting up the
  type system, among other things.
*/
void
SoMFInt32::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(MFInt32)
  // Make sure we only initialize once.
  assert(SoMFInt32::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFInt32::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "MFInt32", &SoMFInt32::createInstance);
//$ END TEMPLATE FieldInitClass
}

// *************************************************************************

SbBool
SoMFInt32::read1Value(SoInput * in, int idx)
{
  SoSFInt32 sfint32;
  SbBool result;
  if ((result = sfint32.readValue(in)))
    this->set1Value(idx, sfint32.getValue());
  return result;
}

void
SoMFInt32::write1Value(SoOutput * out, int idx) const
{
  SoSFInt32 sfint32;
  sfint32.setValue((*this)[idx]);
  sfint32.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
int
SoMFInt32::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFInt32::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFInt32::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFInt32 *)dest)->setValue((*this)[0]);
  }
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    const int num=this->getNum();
    ostrstream ostr;
    if (num!=1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      ostr << (*this)[i];
      if (i<num-1) ostr << ", ";
    }
    if (num!=1) ostr << " ]";
    ostr << ends; 
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFInt32::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
