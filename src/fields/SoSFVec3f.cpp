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
  \class SoSFVec3f SoSFVec3f.h Inventor/fields/SoSFVec3f.h
  \brief The SoSFVec3f class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFString.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32

// *************************************************************************

//$ BEGIN TEMPLATE SField(SoSFVec3f, const SbVec3f &)

SoType SoSFVec3f::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFVec3f::getTypeId(void) const
{
  return SoSFVec3f::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFVec3f class.

  \sa getTypeId(), SoType
 */
SoType
SoSFVec3f::getClassTypeId(void)
{
  return SoSFVec3f::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFVec3f class.
*/
void *
SoSFVec3f::createInstance(void)
{
  return new SoSFVec3f;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFVec3f::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFVec3f::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFVec3f &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFVec3f::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFVec3f &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFVec3f &
SoSFVec3f::operator = (const SoSFVec3f & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFVec3f::SoSFVec3f(void)
{
  // Make sure we have initialized class.
  assert(SoSFVec3f::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFVec3f::~SoSFVec3f()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFVec3f::setValue(const SbVec3f & value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFVec3f::operator == (const SoSFVec3f & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoSFVec3f class. This includes setting up the
  type system, among other things.
*/
void
SoSFVec3f::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(SFVec3f)
  // Make sure we only initialize once.
  assert(SoSFVec3f::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFVec3f::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SFVec3f", &SoSFVec3f::createInstance);
//$ END TEMPLATE FieldInitClass
}

SbBool
SoSFVec3f::readValue(SoInput * in)
{
  return (in->read(value[0]) && in->read(value[1]) && in->read(value[2]));
}

void
SoSFVec3f::writeValue(SoOutput * out) const
{
  SbVec3f v = this->getValue(); // evaluate

  out->write(v[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(v[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(v[2]);
}

/*!
  FIXME: write function documentation
*/
void
SoSFVec3f::setValue(const float x, const float y, const float z)
{
  this->setValue(SbVec3f(x, y, z));
}

/*!
  FIXME: write function documentation
*/
void
SoSFVec3f::setValue(const float xyz[3])
{
  this->setValue(SbVec3f(xyz));
}

void
SoSFVec3f::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFColor::getClassTypeId()) {
    ((SoSFColor *)dest)->setValue(this->getValue().getValue());
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    ostr << this->getValue()[0] << " " <<
      this->getValue()[1] << " " <<
      this->getValue()[2] << '\0';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
  else if (dest->getTypeId()==SoMFVec3f::getClassTypeId()) {
    ((SoMFVec3f *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFVec3f::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
