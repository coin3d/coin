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
  \class SoSFPlane SoSFPlane.h Inventor/fields/SoSFPlane.h
  \brief The SoSFPlane class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFPlane.h>
#if !defined(COIN_EXCLUDE_SOMFPLANE)
#include <Inventor/fields/SoMFPlane.h>
#endif // !COIN_EXCLUDE_SOMFPLANE
#include <Inventor/SoOutput.h>
#include <Inventor/SoInput.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <strstream.h>

// *************************************************************************

//$ BEGIN TEMPLATE SField( SoSFPlane, const SbPlane & )

SoType SoSFPlane::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFPlane::getTypeId(void) const
{
  return SoSFPlane::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFPlane class.

  \sa getTypeId(), SoType
 */
SoType
SoSFPlane::getClassTypeId(void)
{
  return SoSFPlane::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFPlane class.
*/
void *
SoSFPlane::createInstance(void)
{
  return new SoSFPlane;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFPlane::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFPlane::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFPlane &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFPlane::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFPlane &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFPlane &
SoSFPlane::operator = (const SoSFPlane & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFPlane::SoSFPlane(void)
{
  // Make sure we have initialized class.
  assert(SoSFPlane::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFPlane::~SoSFPlane()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFPlane::setValue(const SbPlane & value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFPlane::operator == (const SoSFPlane & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField

/*!
  Does initialization common for all objects of the
  SoSFPlane class. This includes setting up the
  type system, among other things.
*/
void
SoSFPlane::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFPlane )
  // Make sure we only initialize once.
  assert(SoSFPlane::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFPlane::classTypeId =
    SoType::createType(SoSFPlane::getClassTypeId(),
                       "SFPlane", &SoSFPlane::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFPlane::cleanClass(void)
{
}

SbBool
SoSFPlane::readValue(SoInput * in)
{
  assert(!in->isBinary() && "FIXME: not implemented");

  SbVec3f normal;
  float offset;
  SbBool result = (in->read(normal[0]) && in->read(normal[1]) &&
		   in->read(normal[2]) &&
		   in->read(offset));
  this->setValue(SbPlane(normal, offset));
  return result;
}

void
SoSFPlane::writeValue(SoOutput * out) const
{
  assert(!out->isBinary() && "FIXME: not implemented");

  out->write(this->value.getNormal()[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(this->value.getNormal()[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(this->value.getNormal()[2]);
  if(!out->isBinary()) out->write(' ');
  out->write(this->value.getDistanceFromOrigin());
}

void
SoSFPlane::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOMFPLANE)
  else if (dest->getTypeId()==SoMFPlane::getClassTypeId()) {
    ((SoMFPlane *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFPLANE
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    const SbPlane plane=this->getValue();
    const SbVec3f norm=plane.getNormal();
    
    ostr << norm[0] << " " << norm[1] << " " << norm[2] << "  " <<
      plane.getDistanceFromOrigin() << ends;

    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFPlane::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
