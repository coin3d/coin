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
  \class SoMFVec2f SoMFVec2f.h Inventor/fields/SoMFVec2f.h
  \brief The SoMFVec2f class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoSFVec2f.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE MField(SoMFVec2f, SbVec2f, const SbVec2f &)

SoType SoMFVec2f::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFVec2f::getTypeId(void) const
{
  return SoMFVec2f::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFVec2f class.

  \sa getTypeId(), SoType
 */
SoType
SoMFVec2f::getClassTypeId(void)
{
  return SoMFVec2f::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFVec2f class.
*/
void *
SoMFVec2f::createInstance(void)
{
  return new SoMFVec2f;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFVec2f::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFVec2f::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFVec2f &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFVec2f::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFVec2f &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFVec2f &
SoMFVec2f::operator = (const SoMFVec2f & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFVec2f::SoMFVec2f(void)
{
  // Make sure we have initialized class.
  assert(SoMFVec2f::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFVec2f::~SoMFVec2f(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFVec2f::fieldSizeof(void) const
{
  return sizeof(SbVec2f);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFVec2f::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::setValuesPtr(void * ptr)
{
  this->values = (SbVec2f *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFVec2f::find(const SbVec2f & value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::setValues(const int start, const int num,
                       const SbVec2f * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SbVec2f) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::set1Value(const int idx, const SbVec2f & value)
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
SoMFVec2f::setValue(const SbVec2f & value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFVec2f::operator == (const SoMFVec2f & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SbVec2f * const lhs = this->getValues(0);
  const SbVec2f * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFVec2f class. This includes setting up the
  type system, among other things.
*/
void
SoMFVec2f::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(MFVec2f)
  // Make sure we only initialize once.
  assert(SoMFVec2f::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFVec2f::classTypeId =
    SoType::createType(SoMFVec2f::getClassTypeId(),
                       "MFVec2f", &SoMFVec2f::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMFVec2f::cleanClass(void)
{
}

// *************************************************************************

SbBool
SoMFVec2f::read1Value(SoInput * in, int idx)
{
  SoSFVec2f sfvec2f;
  SbBool result;
  if ((result = sfvec2f.readValue(in)))
    this->set1Value(idx, sfvec2f.getValue());
  return result;
}

void
SoMFVec2f::write1Value(SoOutput * out, int idx) const
{
  SoSFVec2f sfvec2f;
  sfvec2f.setValue((*this)[idx]);
  sfvec2f.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::setValues(const int start, const int num, const float xy[][2])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->set1Value(start+i, SbVec2f(xy[i]));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::set1Value(const int idx, const float x, const float y)
{
  this->set1Value(idx, SbVec2f(x, y));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::set1Value(const int idx, const float xy[2])
{
  this->set1Value(idx, SbVec2f(xy));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::setValue(const float x, const float y)
{
  this->setValue(SbVec2f(x, y));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::setValue(const float xy[2])
{
  if (xy == NULL) this->allocValues(0);
  else this->setValue(SbVec2f(xy[0], xy[1]));
}

void
SoMFVec2f::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFVec2f::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFVec2f *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFVec2f::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
