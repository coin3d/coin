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
  \class SoMFVec4f SoMFVec4f.h Inventor/fields/SoMFVec4f.h
  \brief The SoMFVec4f class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFVec4f.h>
#include <Inventor/fields/SoSFVec4f.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE MField(SoMFVec4f, SbVec4f, const SbVec4f &)

SoType SoMFVec4f::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFVec4f::getTypeId(void) const
{
  return SoMFVec4f::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFVec4f class.

  \sa getTypeId(), SoType
 */
SoType
SoMFVec4f::getClassTypeId(void)
{
  return SoMFVec4f::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFVec4f class.
*/
void *
SoMFVec4f::createInstance(void)
{
  return new SoMFVec4f;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFVec4f::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFVec4f::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFVec4f &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFVec4f::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFVec4f &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFVec4f &
SoMFVec4f::operator = (const SoMFVec4f & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFVec4f::SoMFVec4f(void)
{
  // Make sure we have initialized class.
  assert(SoMFVec4f::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFVec4f::~SoMFVec4f(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFVec4f::fieldSizeof(void) const
{
  return sizeof(SbVec4f);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFVec4f::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::setValuesPtr(void * ptr)
{
  this->values = (SbVec4f *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFVec4f::find(const SbVec4f & value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::setValues(const int start, const int num,
                       const SbVec4f * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SbVec4f) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::set1Value(const int idx, const SbVec4f & value)
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
SoMFVec4f::setValue(const SbVec4f & value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFVec4f::operator == (const SoMFVec4f & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SbVec4f * const lhs = this->getValues(0);
  const SbVec4f * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFVec4f class. This includes setting up the
  type system, among other things.
*/
void
SoMFVec4f::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(MFVec4f)
  // Make sure we only initialize once.
  assert(SoMFVec4f::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFVec4f::classTypeId =
    SoType::createType(SoMFVec4f::getClassTypeId(),
                       "MFVec4f", &SoMFVec4f::createInstance);
//$ END TEMPLATE FieldInitClass
}

// *************************************************************************

SbBool
SoMFVec4f::read1Value(SoInput * in, int idx)
{
  SoSFVec4f sfvec4f;
  SbBool result;
  if ((result = sfvec4f.readValue(in)))
    this->set1Value(idx, sfvec4f.getValue());
  return result;
}

void
SoMFVec4f::write1Value(SoOutput * out, int idx) const
{
  SoSFVec4f sfvec4f;
  sfvec4f.setValue((*this)[idx]);
  sfvec4f.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::setValues(const int start, const int num, const float xyzw[][4])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[i+start].setValue(xyzw[i]);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::set1Value(const int idx, const float x, const float y,
		      const float z, const float w)
{
  this->set1Value(idx, SbVec4f(x, y, z, w));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::set1Value(const int idx, const float xyzw[4])
{
  this->set1Value(idx, SbVec4f(xyzw));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::setValue(const float x, const float y,
		     const float z, const float w)
{
  this->setValue(SbVec4f(x,y,z,w));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::setValue(const float xyzw[4])
{
  if (xyzw == NULL) this->allocValues(0);
  else this->setValue(SbVec4f(xyzw));
}

void
SoMFVec4f::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFVec4f::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFVec4f *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFVec4f::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
