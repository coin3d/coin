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
  \class SoMFRotation SoMFRotation.h Inventor/fields/SoMFRotation.h
  \brief The SoMFRotation class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFRotation.h>
#if !defined(COIN_EXCLUDE_SOSFROTATION)
#include <Inventor/fields/SoSFRotation.h>
#endif // !COIN_EXCLUDE_SOSFROTATION
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/SbVec3f.h>
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#include <strstream.h>

// *************************************************************************

//$ BEGIN TEMPLATE MField( SoMFRotation, SbRotation, const SbRotation & )

SoType SoMFRotation::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFRotation::getTypeId(void) const
{
  return SoMFRotation::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFRotation class.

  \sa getTypeId(), SoType
 */
SoType
SoMFRotation::getClassTypeId(void)
{
  return SoMFRotation::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFRotation class.
*/
void *
SoMFRotation::createInstance(void)
{
  return new SoMFRotation;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFRotation::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFRotation::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFRotation &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFRotation::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFRotation &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFRotation &
SoMFRotation::operator = (const SoMFRotation & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFRotation::SoMFRotation(void)
{
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFRotation::~SoMFRotation(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFRotation::fieldSizeof(void) const
{
  return sizeof(SbRotation);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFRotation::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::setValuesPtr(void * ptr)
{
  this->values = (SbRotation *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFRotation::find(const SbRotation & value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::setValues(const int start, const int num,
                       const SbRotation * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SbRotation) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::set1Value(const int idx, const SbRotation & value)
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
SoMFRotation::setValue(const SbRotation & value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFRotation::operator == (const SoMFRotation & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SbRotation * const lhs = this->getValues(0);
  const SbRotation * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFRotation class. This includes setting up the
  type system, among other things.
*/
void
SoMFRotation::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( MFRotation )
  // Make sure we only initialize once.
  assert(SoMFRotation::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFRotation::classTypeId =
    SoType::createType(SoMFRotation::getClassTypeId(),
                       "MFRotation", &SoMFRotation::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMFRotation::cleanClass(void)
{
}

// *************************************************************************

/*!
  FIXME: write function documentation
*/
SbBool
SoMFRotation::read1Value(SoInput * in, int idx)
{
  assert(!in->isBinary() && "FIXME: not implemented");

  float a[3];
  float r;

  SbBool result =
    in->read(a[0]) && in->read(a[1]) && in->read(a[2]) && in->read(r);

  if(result) this->values[idx].setValue(SbVec3f(a), r);
  return result;
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::write1Value(SoOutput * out, int idx) const
{
  assert(!out->isBinary() && "FIXME: not implemented");

  SbVec3f axis;
  float rotation;

  this->values[idx].getValue(axis, rotation);

  out->write(axis[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(axis[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(axis[2]);
  if(!out->isBinary()) out->write("  ");
  out->write(rotation);
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::setValues(const int start, const int num, const float q[][4])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[i+start].setValue(q[i]);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::set1Value(const int idx, const float q0, const float q1,
			 const float q2, const float q3)
{
  this->set1Value(idx,SbRotation(q0,q1,q2,q3));
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::set1Value(const int idx, const float q[4])
{
  this->set1Value(idx,SbRotation(q[0],q[1],q[2],q[3]));
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::set1Value(const int idx, const SbVec3f & axis, const float angle)
{
  this->set1Value(idx,SbRotation(axis,angle));
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::setValue(const float q0, const float q1, const float q2, const float q3)
{
  this->setValue(SbRotation(q0,q1,q2,q3));
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::setValue(const float q[4])
{
  this->setValue(SbRotation(q[0],q[1],q[2],q[3]));
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::setValue(const SbVec3f & axis, const float angle)
{
  this->setValue(SbRotation(axis,angle));
}

void
SoMFRotation::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFROTATION)
  else if (dest->getTypeId()==SoSFRotation::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFRotation *)dest)->setValue((*this)[0]);
  }
#endif // !COIN_EXCLUDE_SOSFROTATION
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    const SbRotation * rotation;
    SbVec3f vec;
    float rad;
    const int num=this->getNum();

    if (num>1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      (*this)[i].getValue(vec,rad);
      ostr << vec[0] << " " << vec[1] << " " << vec[2] << "  " <<
	rad;
      if (i<num-1) ostr << ", ";
    }
    if (num>1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFRotation::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
