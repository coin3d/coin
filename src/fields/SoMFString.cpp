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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoMFString SoMFString.h Inventor/fields/SoMFString.h
  \brief The SoMFString class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE MField( SoMFString, SbString, const SbString & )

SoType SoMFString::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFString::getTypeId(void) const
{
  return SoMFString::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFString class.

  \sa getTypeId(), SoType
 */
SoType
SoMFString::getClassTypeId(void)
{
  return SoMFString::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFString class.
*/
void *
SoMFString::createInstance(void)
{
  return new SoMFString;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFString::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFString::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFString &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFString::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFString &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFString &
SoMFString::operator = (const SoMFString & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFString::SoMFString(void)
{
  // Make sure we have initialized class.
  assert(SoMFString::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFString::~SoMFString(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFString::fieldSizeof(void) const
{
  return sizeof(SbString);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFString::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::setValuesPtr(void * ptr)
{
  this->values = (SbString *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFString::find(const SbString & value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::setValues(const int start, const int num,
                       const SbString * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SbString) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::set1Value(const int idx, const SbString & value)
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
SoMFString::setValue(const SbString & value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFString::operator == (const SoMFString & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SbString * const lhs = this->getValues(0);
  const SbString * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField
//$ BEGIN TEMPLATE MFieldAlloc( SoMFString, SbString )
/*!
  Allocate \a number value slots in this field.
*/
void
SoMFString::allocValues(int number)
{
  assert(number >= 0);

  if (number == 0) {
    delete[] this->values;
    this->values = NULL;
  }
  else {
    if (this->values) {
      // FIXME: this is extremely ineffective if allocValues() is called
      // repeatedly with a number value which is 1 higher than the last
      // (which is often the case). 990417 mortene.
      SbString * newblock = new SbString[number];
      for (int i=0; i < SbMin(this->num, number); i++)
        newblock[i] = this->values[i];

      delete[] this->values;
      this->values = newblock;
    }
    else {
      this->values = new SbString[number];
    }
  }

  SbBool valchanged = number < this->num ? TRUE : FALSE;
  this->num = number;
  this->maxNum = number;
  if (valchanged) this->valueChanged();
}

//$ END TEMPLATE MFieldAlloc

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFString class. This includes setting up the
  type system, among other things.
*/
void
SoMFString::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( MFString )
  // Make sure we only initialize once.
  assert(SoMFString::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFString::classTypeId =
    SoType::createType(SoMFString::getClassTypeId(),
                       "MFString", &SoMFString::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMFString::cleanClass(void)
{
}

// *************************************************************************

SbBool
SoMFString::read1Value(SoInput * in, int idx)
{
  SoSFString sfstring;
  SbBool result;
  if (result = sfstring.readValue(in)) this->set1Value(idx, sfstring.getValue());
  return result;
}

void
SoMFString::write1Value(SoOutput * out, int idx) const
{
  SoSFString sfstring;
  sfstring.setValue((*this)[idx]);
  sfstring.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::setValues(const int start, const int num,
		     const char * const strings [])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;
  
  for (int i=0;i<num;i++) this->set1Value(i+start,SbString(strings[i]));
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::setValue(const char * const string)
{
  this->setValue(SbString(string));
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::deleteText(const int fromLine, const int fromChar,
			const int toLine, const int toChar)
{
  assert(0 && "FIXME: not implemented yet");
}

void
SoMFString::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFString *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFString::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
