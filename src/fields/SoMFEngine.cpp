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
  \class SoMFEngine SoMFEngine.h Inventor/fields/SoMFEngine.h
  \brief The SoMFEngine class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFEngine.h>
#include <Inventor/fields/SoSFEngine.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SbName.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <assert.h>

// *************************************************************************

//$ BEGIN TEMPLATE MField(SoMFEngine, SoEngine *, SoEngine *)

SoType SoMFEngine::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFEngine::getTypeId(void) const
{
  return SoMFEngine::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFEngine class.

  \sa getTypeId(), SoType
 */
SoType
SoMFEngine::getClassTypeId(void)
{
  return SoMFEngine::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFEngine class.
*/
void *
SoMFEngine::createInstance(void)
{
  return new SoMFEngine;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFEngine::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFEngine::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFEngine &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFEngine::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFEngine &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFEngine &
SoMFEngine::operator = (const SoMFEngine & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFEngine::SoMFEngine(void)
{
  // Make sure we have initialized class.
  assert(SoMFEngine::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFEngine::~SoMFEngine(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFEngine::fieldSizeof(void) const
{
  return sizeof(SoEngine *);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFEngine::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFEngine::setValuesPtr(void * ptr)
{
  this->values = (SoEngine * *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFEngine::find(SoEngine * value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFEngine::setValues(const int start, const int num,
                       const SoEngine * * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SoEngine *) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFEngine::set1Value(const int idx, SoEngine * value)
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
SoMFEngine::setValue(SoEngine * value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFEngine::operator == (const SoMFEngine & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SoEngine * * const lhs = this->getValues(0);
  const SoEngine * * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFEngine::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFEngine::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFEngine class. This includes setting up the
  type system, among other things.
*/
void
SoMFEngine::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(MFEngine)
  // Make sure we only initialize once.
  assert(SoMFEngine::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFEngine::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "MFEngine", &SoMFEngine::createInstance);
//$ END TEMPLATE FieldInitClass
}

// *************************************************************************

/*!
  FIXME: write function documentation
*/
SbBool
SoMFEngine::read1Value(SoInput * /* in */, int /* idx */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFEngine::write1Value(SoOutput * /* out */, int /* idx */) const
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoMFEngine::fixCopy(SbBool /* copyConnections */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFEngine::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

void
SoMFEngine::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFEngine::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFEngine *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFEngine::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
