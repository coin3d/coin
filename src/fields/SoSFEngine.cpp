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
  \class SoSFEngine SoSFEngine.h Inventor/fields/SoSFEngine.h
  \brief The SoSFEngine class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFEngine.h>
#if !defined(COIN_EXCLUDE_SOMFENGINE)
#include <Inventor/fields/SoMFEngine.h>
#endif // !COIN_EXCLUDE_SOMFENGINE
#if !defined(COIN_EXCLUDE_SOENGINE)
#include <Inventor/engines/SoEngine.h>
#endif // !COIN_EXCLUDE_SOENGINE
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE SFieldRequired(SoSFEngine)

SoType SoSFEngine::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFEngine::getTypeId(void) const
{
  return SoSFEngine::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFEngine class.

  \sa getTypeId(), SoType
 */
SoType
SoSFEngine::getClassTypeId(void)
{
  return SoSFEngine::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFEngine class.
*/
void *
SoSFEngine::createInstance(void)
{
  return new SoSFEngine;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFEngine::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFEngine::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFEngine &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFEngine::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFEngine &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFEngine &
SoSFEngine::operator = (const SoSFEngine & field)
{
  this->setValue(field.getValue());
  return *this;
}
//$ END TEMPLATE SFieldRequired

/*!
  FIXME: write function documentation
*/
void
SoSFEngine::setValue(SoEngine * value)
{
    this->value = value;
    valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFEngine::operator == (const SoSFEngine & field) const
{
    return (getValue() == field.getValue());
}

/*!
  Does initialization common for all objects of the
  SoSFEngine class. This includes setting up the
  type system, among other things.
*/
void
SoSFEngine::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(SFEngine)
  // Make sure we only initialize once.
  assert(SoSFEngine::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFEngine::classTypeId =
    SoType::createType(SoSFEngine::getClassTypeId(),
                       "SFEngine", &SoSFEngine::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFEngine::cleanClass(void)
{
}

/*!
  Constructor.
*/
SoSFEngine::SoSFEngine(void)
{
  this->value = NULL;
}

/*!
  Destructor.
*/
SoSFEngine::~SoSFEngine(void)
{
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFEngine::readValue(SoInput * /* in */)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFEngine::fixCopy(SbBool /* copyConnections */)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFEngine::referencesCopy(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFEngine::writeValue(SoOutput * /* out */) const
{
  assert(0 && "FIXME: not implemented yet");
}

void
SoSFEngine::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOMFENGINE)
  else if (dest->getTypeId()==SoMFEngine::getClassTypeId()) {
    ((SoMFEngine *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFENGINE
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFEngine::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
