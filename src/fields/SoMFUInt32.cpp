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
  \class SoMFUInt32 SoMFUInt32.h Inventor/fields/SoMFUInt32.h
  \brief The SoMFUInt32 class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFUInt32.h>
#if !defined(COIN_EXCLUDE_SOSFUINT32)
#include <Inventor/fields/SoSFUInt32.h>
#endif // !COIN_EXCLUDE_SOSFUINT32
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#include <strstream.h>
#include <iomanip.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE MField( SoMFUInt32, uint32_t, uint32_t )

SoType SoMFUInt32::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFUInt32::getTypeId(void) const
{
  return SoMFUInt32::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFUInt32 class.

  \sa getTypeId(), SoType
 */
SoType
SoMFUInt32::getClassTypeId(void)
{
  return SoMFUInt32::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFUInt32 class.
*/
void *
SoMFUInt32::createInstance(void)
{
  return new SoMFUInt32;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFUInt32::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFUInt32::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFUInt32 &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFUInt32::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFUInt32 &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFUInt32 &
SoMFUInt32::operator = (const SoMFUInt32 & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFUInt32::SoMFUInt32(void)
{
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFUInt32::~SoMFUInt32(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFUInt32::fieldSizeof(void) const
{
  return sizeof(uint32_t);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFUInt32::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFUInt32::setValuesPtr(void * ptr)
{
  this->values = (uint32_t *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFUInt32::find(uint32_t value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFUInt32::setValues(const int start, const int num,
                       const uint32_t * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (uint32_t) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFUInt32::set1Value(const int idx, uint32_t value)
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
SoMFUInt32::setValue(uint32_t value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFUInt32::operator == (const SoMFUInt32 & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const uint32_t * const lhs = this->getValues(0);
  const uint32_t * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFUInt32::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFUInt32::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFUInt32 class. This includes setting up the
  type system, among other things.
*/
void
SoMFUInt32::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( MFUInt32 )
  // Make sure we only initialize once.
  assert(SoMFUInt32::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFUInt32::classTypeId =
    SoType::createType(SoMFUInt32::getClassTypeId(),
                       "MFUInt32", &SoMFUInt32::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMFUInt32::cleanClass(void)
{
}


// *************************************************************************

/*!
  FIXME: write function documentation
*/
SbBool
SoMFUInt32::read1Value(SoInput * in, int index)
{
  return in->read(values[index]);
}

/*!
  FIXME: write function documentation
*/
void
SoMFUInt32::write1Value(SoOutput * out, int idx) const
{
  out->write(this->values[idx]);
}

/*!
  FIXME: write function documentation
*/
int
SoMFUInt32::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFUInt32::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFUINT32)
  else if (dest->getTypeId()==SoSFUInt32::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFUInt32 *)dest)->setValue((*this)[0]);
  }
#endif // !COIN_EXCLUDE_SOSFUINT32
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
    SoDebugError::post("SoMFUInt32::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
