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
  \class SoSFVec2f SoSFVec2f.h Inventor/fields/SoSFVec2f.h
  \brief The SoSFVec2f class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFVec2f.h>
#if !defined(COIN_EXCLUDE_SOMFVEC2F)
#include <Inventor/fields/SoMFVec2f.h>
#endif // !COIN_EXCLUDE_SOMFVEC2F
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE SField( SoSFVec2f, const SbVec2f & )

SoType SoSFVec2f::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFVec2f::getTypeId(void) const
{
  return SoSFVec2f::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFVec2f class.

  \sa getTypeId(), SoType
 */
SoType
SoSFVec2f::getClassTypeId(void)
{
  return SoSFVec2f::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFVec2f class.
*/
void *
SoSFVec2f::createInstance(void)
{
  return new SoSFVec2f;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFVec2f::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFVec2f::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFVec2f &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFVec2f::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFVec2f &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFVec2f &
SoSFVec2f::operator = (const SoSFVec2f & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFVec2f::SoSFVec2f(void)
{
  // Make sure we have initialized class.
  assert(SoSFVec2f::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFVec2f::~SoSFVec2f()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFVec2f::setValue(const SbVec2f & value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFVec2f::operator == (const SoSFVec2f & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

/*!
  Does initialization common for all objects of the
  SoSFVec2f class. This includes setting up the
  type system, among other things.
*/
void
SoSFVec2f::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFVec2f )
  // Make sure we only initialize once.
  assert(SoSFVec2f::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFVec2f::classTypeId =
    SoType::createType(SoSFVec2f::getClassTypeId(),
                       "SFVec2f", &SoSFVec2f::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFVec2f::cleanClass(void)
{
}

SbBool
SoSFVec2f::readValue(SoInput * in)
{
  return (in->read(value[0]) && in->read(value[1]));
}

void
SoSFVec2f::writeValue(SoOutput * out) const
{
  SbVec2f v = this->getValue(); // evaluate

  out->write(v[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(v[1]);
}

/*!
  FIXME: write function documentation
*/
void
SoSFVec2f::setValue(const float x, const float y)
{
  this->setValue(SbVec2f(x, y));
}

/*!
  FIXME: write function documentation
*/
void
SoSFVec2f::setValue(const float xy[2])
{
  this->setValue(SbVec2f(xy));
}

void
SoSFVec2f::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOMFVEC2F)
  else if (dest->getTypeId()==SoMFVec2f::getClassTypeId()) {
    ((SoMFVec2f *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFVEC2F
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFVec2f::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
