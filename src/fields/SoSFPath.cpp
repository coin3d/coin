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
  \class SoSFPath SoSFPath.h Inventor/fields/SoSFPath.h
  \brief The SoSFPath class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFPath.h>
#if !defined(COIN_EXCLUDE_SOMFPATH)
#include <Inventor/fields/SoMFPath.h>
#endif // !COIN_EXCLUDE_SOMFPATH
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE SFieldRequired( SoSFPath )

SoType SoSFPath::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFPath::getTypeId(void) const
{
  return SoSFPath::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFPath class.

  \sa getTypeId(), SoType
 */
SoType
SoSFPath::getClassTypeId(void)
{
  return SoSFPath::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFPath class.
*/
void *
SoSFPath::createInstance(void)
{
  return new SoSFPath;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFPath::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFPath::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFPath &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFPath::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFPath &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFPath &
SoSFPath::operator = (const SoSFPath & field)
{
  this->setValue(field.getValue());
  return *this;
}
//$ END TEMPLATE SFieldRequired

// *************************************************************************

/*!
  FIXME: write function documentation
*/
void
SoSFPath::setValue(SoPath * value)
{
    this->value = value;
    valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFPath::operator == (const SoSFPath & field) const
{
    return (getValue() == field.getValue());
}

/*!
  Does initialization common for all objects of the
  SoSFPath class. This includes setting up the
  type system, among other things.
*/
void
SoSFPath::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFPath )
  // Make sure we only initialize once.
  assert(SoSFPath::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFPath::classTypeId =
    SoType::createType(SoSFPath::getClassTypeId(),
                       "SFPath", &SoSFPath::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFPath::cleanClass(void)
{
}

/*!
  Constructor.
*/
SoSFPath::SoSFPath(void)
{
  this->value = NULL;
}

/*!
  Destructor.
*/
SoSFPath::~SoSFPath(void)
{
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFPath::readValue(SoInput * in)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFPath::notify(SoNotList * list)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoSFPath::fixCopy(SbBool copyConnections)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFPath::referencesCopy(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFPath::writeValue(SoOutput * out) const
{
  assert(0 && "FIXME: not implemented yet");
}

void
SoSFPath::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOMFPATH)
  else if (dest->getTypeId()==SoMFPath::getClassTypeId()) {
    ((SoMFPath *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFPATH
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFPath::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
