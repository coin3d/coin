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
  \class SoMFPlane SoMFPlane.h Inventor/fields/SoMFPlane.h
  \brief The SoMFPlane class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFPlane.h>
#include <Inventor/fields/SoSFPlane.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#include <strstream.h>
// *************************************************************************

//$ BEGIN TEMPLATE MField( SoMFPlane, SbPlane, const SbPlane & )

SoType SoMFPlane::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFPlane::getTypeId(void) const
{
  return SoMFPlane::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFPlane class.

  \sa getTypeId(), SoType
 */
SoType
SoMFPlane::getClassTypeId(void)
{
  return SoMFPlane::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFPlane class.
*/
void *
SoMFPlane::createInstance(void)
{
  return new SoMFPlane;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFPlane::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFPlane::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFPlane &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFPlane::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFPlane &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFPlane &
SoMFPlane::operator = (const SoMFPlane & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFPlane::SoMFPlane(void)
{
  // Make sure we have initialized class.
  assert(SoMFPlane::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFPlane::~SoMFPlane(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFPlane::fieldSizeof(void) const
{
  return sizeof(SbPlane);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFPlane::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFPlane::setValuesPtr(void * ptr)
{
  this->values = (SbPlane *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFPlane::find(const SbPlane & value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFPlane::setValues(const int start, const int num,
                       const SbPlane * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SbPlane) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFPlane::set1Value(const int idx, const SbPlane & value)
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
SoMFPlane::setValue(const SbPlane & value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFPlane::operator == (const SoMFPlane & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SbPlane * const lhs = this->getValues(0);
  const SbPlane * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFPlane::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFPlane::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFPlane class. This includes setting up the
  type system, among other things.
*/
void
SoMFPlane::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( MFPlane )
  // Make sure we only initialize once.
  assert(SoMFPlane::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFPlane::classTypeId =
    SoType::createType(SoMFPlane::getClassTypeId(),
                       "MFPlane", &SoMFPlane::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMFPlane::cleanClass(void)
{
}

// *************************************************************************

SbBool
SoMFPlane::read1Value(SoInput * in, int idx)
{
  SoSFPlane sfplane;
  SbBool result;
  if (result = sfplane.readValue(in)) this->set1Value(idx, sfplane.getValue());
  return result;
}

void
SoMFPlane::write1Value(SoOutput * out, int idx) const
{
  SoSFPlane sfplane;
  sfplane.setValue((*this)[idx]);
  sfplane.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
int
SoMFPlane::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFPlane::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFPlane::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFPlane *)dest)->setValue((*this)[0]);
  }
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    const SbPlane * plane;
    const int num=this->getNum();

    if (num>1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      plane=&(*this)[i];
      const SbVec3f * norm=&plane->getNormal();
      ostr << (*norm)[0] << " " << (*norm)[1] << " " << (*norm)[2] << "  " <<
	plane->getDistanceFromOrigin();
      if (i<num-1) ostr << ", ";
    }
    if (num>1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFPlane::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
