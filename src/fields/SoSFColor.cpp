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
  \class SoSFColor SoSFColor.h Inventor/fields/SoSFColor.h
  \brief The SoSFColor class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <math.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOSFVEC3F)
#include <Inventor/fields/SoSFVec3f.h>
#endif // !COIN_EXCLUDE_SOSFVEC3F
#if !defined(COIN_EXCLUDE_SOMFCOLOR)
#include <Inventor/fields/SoMFColor.h>
#endif // !COIN_EXCLUDE_SOMFCOLOR
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#include <strstream.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE SField( SoSFColor, const SbColor & )

SoType SoSFColor::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFColor::getTypeId(void) const
{
  return SoSFColor::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFColor class.

  \sa getTypeId(), SoType
 */
SoType
SoSFColor::getClassTypeId(void)
{
  return SoSFColor::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFColor class.
*/
void *
SoSFColor::createInstance(void)
{
  return new SoSFColor;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFColor::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFColor::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFColor &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFColor::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFColor &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFColor &
SoSFColor::operator = (const SoSFColor & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFColor::SoSFColor(void)
{
}

/*!
  Destructor.
*/
SoSFColor::~SoSFColor()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFColor::setValue(const SbColor & value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFColor::operator == (const SoSFColor & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField


/*!
  Does initialization common for all objects of the
  SoSFColor class. This includes setting up the
  type system, among other things.
*/
void
SoSFColor::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFColor )
  // Make sure we only initialize once.
  assert(SoSFColor::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFColor::classTypeId =
    SoType::createType(SoSFColor::getClassTypeId(),
                       "SFColor", &SoSFColor::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFColor::cleanClass(void)
{
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFColor::readValue(SoInput * in)
{
  if (in->read(value[0]) && in->read(value[1]) && in->read(value[2])) {
    return TRUE;
  }
  else return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::writeValue(SoOutput * out) const
{
  out->write(this->value[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(this->value[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(this->value[2]);
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::setValue(const SbVec3f & vec)
{
  value.setValue(vec[0], vec[1], vec[2]);
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::setValue(const float red, const float green, const float blue)
{
  value.setValue(red, green, blue);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::setValue(const float rgb[3])
{
  value.setValue(rgb);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::setHSVValue(const float h, const float s, const float v)
{
  value.setHSVValue(h, s, v);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::setHSVValue(const float hsv[3])
{
  value.setHSVValue(hsv);
  this->valueChanged();
}

void
SoSFColor::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFVEC3F)
  else if (dest->getTypeId()==SoSFVec3f::getClassTypeId()) {
    ((SoSFVec3f *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFVEC3F
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    ostr << this->getValue()[0] << " " <<
      this->getValue()[1] << " " <<
      this->getValue()[2] << '\0';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFCOLOR)
  else if (dest->getTypeId()==SoMFColor::getClassTypeId()) {
    ((SoMFColor *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFCOLOR
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFColor::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
