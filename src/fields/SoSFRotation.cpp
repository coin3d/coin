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
  \class SoSFRotation SoSFRotation.h Inventor/fields/SoSFRotation.h
  \brief The SoSFRotation class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/SbVec3f.h>

#if !defined(COIN_EXCLUDE_SOSFMATRIX)
#include <Inventor/fields/SoSFMatrix.h>
#endif // !COIN_EXCLUDE_SOSFMATRIX
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFROTATION)
#include <Inventor/fields/SoMFRotation.h>
#endif // !COIN_EXCLUDE_SOMFROTATION

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32

// *************************************************************************

//$ BEGIN TEMPLATE SField(SoSFRotation, const SbRotation &)

SoType SoSFRotation::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFRotation::getTypeId(void) const
{
  return SoSFRotation::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFRotation class.

  \sa getTypeId(), SoType
 */
SoType
SoSFRotation::getClassTypeId(void)
{
  return SoSFRotation::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFRotation class.
*/
void *
SoSFRotation::createInstance(void)
{
  return new SoSFRotation;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFRotation::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFRotation::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFRotation &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFRotation::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFRotation &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFRotation &
SoSFRotation::operator = (const SoSFRotation & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFRotation::SoSFRotation(void)
{
  // Make sure we have initialized class.
  assert(SoSFRotation::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFRotation::~SoSFRotation()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFRotation::setValue(const SbRotation & value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFRotation::operator == (const SoSFRotation & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

/*!
  Does initialization common for all objects of the
  SoSFRotation class. This includes setting up the
  type system, among other things.
*/
void
SoSFRotation::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(SFRotation)
  // Make sure we only initialize once.
  assert(SoSFRotation::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFRotation::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SFRotation", &SoSFRotation::createInstance);
//$ END TEMPLATE FieldInitClass
}

SbBool
SoSFRotation::readValue(SoInput * in)
{
  SbVec3f axis;
  float angle;
  SbBool result = (in->read(axis[0]) &&
		   in->read(axis[1]) &&
		   in->read(axis[2]) &&
		   in->read(angle));
  this->setValue(axis, angle);
  return result;
}

void
SoSFRotation::writeValue(SoOutput * out) const
{
  SbVec3f axis;
  float angle;

  this->getValue(axis, angle);

  // Handle invalid rotation specifications.
  if (axis.length() == 0.0f) {
    axis.setValue(0.0f, 0.0f, 1.0f);
    angle = 0.0f;
  }

  out->write(axis[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(axis[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(axis[2]);
  if(!out->isBinary()) out->write("  ");
  out->write(angle);
}

/*!
  FIXME: write function documentation
*/
void
SoSFRotation::getValue(SbVec3f & axis, float & angle) const
{
  evaluate();
  this->value.getValue(axis, angle);
}

/*!
  FIXME: write function documentation
*/
void
SoSFRotation::setValue(const float q0, const float q1,
			const float q2, const float q3)
{
  this->setValue(SbRotation(q0, q1, q2, q3));
}

/*!
  FIXME: write function documentation
*/
void
SoSFRotation::setValue(const float q[4])
{
  this->setValue(SbRotation(q));
}

/*!
  FIXME: write function documentation
*/
void
SoSFRotation::setValue(const SbVec3f & axis, const float angle)
{
  this->setValue(SbRotation(axis, angle));
}

void
SoSFRotation::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFMATRIX)
  else if (dest->getTypeId()==SoSFMatrix::getClassTypeId()) {
    SbMatrix mat;
    mat.setRotate(this->getValue());
    ((SoSFMatrix *)dest)->setValue(mat);
  }
#endif // !COIN_EXCLUDE_SOSFMATRIX
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    SbVec3f vec;
    float rad;
    this->getValue().getValue(vec,rad);
    ostr << vec[0] << " " << vec[1] << " " << vec[2] << "  " <<
      rad << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFROTATION)
  else if (dest->getTypeId()==SoMFRotation::getClassTypeId()) {
    ((SoMFRotation *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFROTATION
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFRotation::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
