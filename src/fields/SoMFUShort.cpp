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
  \class SoMFUShort SoMFUShort.h Inventor/fields/SoMFUShort.h
  \brief The SoMFUShort class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFUShort.h>
#include <Inventor/fields/SoSFUShort.h>

#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <assert.h>
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#ifdef _WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif
#include <iomanip.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE MField(SoMFUShort, unsigned short, unsigned short)

SoType SoMFUShort::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFUShort::getTypeId(void) const
{
  return SoMFUShort::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFUShort class.

  \sa getTypeId(), SoType
 */
SoType
SoMFUShort::getClassTypeId(void)
{
  return SoMFUShort::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFUShort class.
*/
void *
SoMFUShort::createInstance(void)
{
  return new SoMFUShort;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFUShort::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFUShort::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFUShort &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFUShort::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFUShort &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFUShort &
SoMFUShort::operator = (const SoMFUShort & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFUShort::SoMFUShort(void)
{
  // Make sure we have initialized class.
  assert(SoMFUShort::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFUShort::~SoMFUShort(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFUShort::fieldSizeof(void) const
{
  return sizeof(unsigned short);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFUShort::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFUShort::setValuesPtr(void * ptr)
{
  this->values = (unsigned short *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFUShort::find(unsigned short value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFUShort::setValues(const int start, const int num,
                       const unsigned short * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (unsigned short) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFUShort::set1Value(const int idx, unsigned short value)
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
SoMFUShort::setValue(unsigned short value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFUShort::operator == (const SoMFUShort & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const unsigned short * const lhs = this->getValues(0);
  const unsigned short * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFUShort::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFUShort::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFUShort class. This includes setting up the
  type system, among other things.
*/
void
SoMFUShort::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(MFUShort)
  // Make sure we only initialize once.
  assert(SoMFUShort::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFUShort::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "MFUShort", &SoMFUShort::createInstance);
//$ END TEMPLATE FieldInitClass
}

// *************************************************************************

SbBool
SoMFUShort::read1Value(SoInput * in, int idx)
{
  SoSFUShort sfushort;
  SbBool result;
  if ((result = sfushort.readValue(in)))
    this->set1Value(idx, sfushort.getValue());
  return result;
}

void
SoMFUShort::write1Value(SoOutput * out, int idx) const
{
  SoSFUShort sfushort;
  sfushort.setValue((*this)[idx]);
  sfushort.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
int
SoMFUShort::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFUShort::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFUShort::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFUShort *)dest)->setValue((*this)[0]);
  }
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    const int num=this->getNum();
    ostrstream ostr;
    if (num!=1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      if ((*this)[i]>0) ostr.flags(ios::hex|ios::showbase);
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
    SoDebugError::post("SoMFUShort::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
