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
  \class SoMFName SoMFName.h Inventor/fields/SoMFName.h
  \brief The SoMFName class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoSFName.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <assert.h>

#include <Inventor/fields/SoSFString.h>
#ifdef _WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

// *************************************************************************

//$ BEGIN TEMPLATE MField(SoMFName, SbName, SbName)

SoType SoMFName::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFName::getTypeId(void) const
{
  return SoMFName::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFName class.

  \sa getTypeId(), SoType
 */
SoType
SoMFName::getClassTypeId(void)
{
  return SoMFName::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFName class.
*/
void *
SoMFName::createInstance(void)
{
  return new SoMFName;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFName::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFName::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFName &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFName::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFName &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFName &
SoMFName::operator = (const SoMFName & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFName::SoMFName(void)
{
  // Make sure we have initialized class.
  assert(SoMFName::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFName::~SoMFName(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFName::fieldSizeof(void) const
{
  return sizeof(SbName);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFName::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFName::setValuesPtr(void * ptr)
{
  this->values = (SbName *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFName::find(SbName value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFName::setValues(const int start, const int num,
                       const SbName * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SbName) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFName::set1Value(const int idx, SbName value)
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
SoMFName::setValue(SbName value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFName::operator == (const SoMFName & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SbName * const lhs = this->getValues(0);
  const SbName * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFName::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFName::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFName class. This includes setting up the
  type system, among other things.
*/
void
SoMFName::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(MFName)
  // Make sure we only initialize once.
  assert(SoMFName::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFName::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "MFName", &SoMFName::createInstance);
//$ END TEMPLATE FieldInitClass
}

// *************************************************************************

SbBool
SoMFName::read1Value(SoInput * in, int idx)
{
  SoSFName sfname;
  SbBool result;
  if ((result = sfname.readValue(in))) this->set1Value(idx, sfname.getValue());
  return result;
}

void
SoMFName::write1Value(SoOutput * out, int idx) const
{
  SoSFName sfname;
  sfname.setValue((*this)[idx]);
  sfname.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFName::setValues(const int start, const int num,
                     const char * const strings [])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++)
    this->values[i+start] = SbName(strings[i]);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFName::setValue(const char * const string)
{
  this->setValue(SbName(string));
}

void
SoMFName::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFName::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFName *)dest)->setValue((*this)[0]);
  }
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    const int num=this->getNum();
    if (num>1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      ostr << '"' << (*this)[i].getString() << '"';
      if (i<num-1) ostr << ", ";
    }
    if (num>1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFName::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
