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
  \class SoMFPath SoMFPath.h Inventor/fields/SoMFPath.h
  \brief The SoMFPath class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFPath.h>
#if !defined(COIN_EXCLUDE_SOSFPATH)
#include <Inventor/fields/SoSFPath.h>
#endif // !COIN_EXCLUDE_SOSFPATH
#include <Inventor/SbName.h>
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE MField(SoMFPath, SoPath *, SoPath *)

SoType SoMFPath::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFPath::getTypeId(void) const
{
  return SoMFPath::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFPath class.

  \sa getTypeId(), SoType
 */
SoType
SoMFPath::getClassTypeId(void)
{
  return SoMFPath::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFPath class.
*/
void *
SoMFPath::createInstance(void)
{
  return new SoMFPath;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFPath::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFPath::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFPath &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFPath::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFPath &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFPath &
SoMFPath::operator = (const SoMFPath & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFPath::SoMFPath(void)
{
  // Make sure we have initialized class.
  assert(SoMFPath::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFPath::~SoMFPath(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFPath::fieldSizeof(void) const
{
  return sizeof(SoPath *);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFPath::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::setValuesPtr(void * ptr)
{
  this->values = (SoPath * *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFPath::find(SoPath * value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::setValues(const int start, const int num,
                       const SoPath * * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SoPath *) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::set1Value(const int idx, SoPath * value)
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
SoMFPath::setValue(SoPath * value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFPath::operator == (const SoMFPath & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SoPath * * const lhs = this->getValues(0);
  const SoPath * * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFPath class. This includes setting up the
  type system, among other things.
*/
void
SoMFPath::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(MFPath)
  // Make sure we only initialize once.
  assert(SoMFPath::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFPath::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "MFPath", &SoMFPath::createInstance);
//$ END TEMPLATE FieldInitClass
}

// *************************************************************************

/*!
  FIXME: write function documentation
*/
SbBool
SoMFPath::read1Value(SoInput * /* in */, int /* idx */)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::write1Value(SoOutput * /* out */, int /* idx */) const
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::notify(SoNotList * /* list */)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::fixCopy(SbBool /* copyConnections */)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFPath::referencesCopy(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

void
SoMFPath::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFPATH)
  else if (dest->getTypeId()==SoSFPath::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFPath *)dest)->setValue((*this)[0]);
  }
#endif // !COIN_EXCLUDE_SOSFPATH
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFPath::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
