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
  \class SoSFTrigger SoSFTrigger.h Inventor/fields/SoSFTrigger.h
  \brief The SoSFTrigger class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>

// *************************************************************************

//$ BEGIN TEMPLATE FieldId(SoSFTrigger)

SoType SoSFTrigger::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFTrigger::getTypeId(void) const
{
  return SoSFTrigger::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFTrigger class.

  \sa getTypeId(), SoType
 */
SoType
SoSFTrigger::getClassTypeId(void)
{
  return SoSFTrigger::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFTrigger class.
*/
void *
SoSFTrigger::createInstance(void)
{
  return new SoSFTrigger;
}
//$ END TEMPLATE FieldId

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoSFTrigger class. This includes setting up the
  type system, among other things.
*/
void
SoSFTrigger::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(SFTrigger)
  // Make sure we only initialize once.
  assert(SoSFTrigger::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFTrigger::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SFTrigger", &SoSFTrigger::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  FIXME: write function documentation
*/
void
SoSFTrigger::copyFrom(const SoField & /* field */)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFTrigger::isSame(const SoField & /* field */) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
const SoSFTrigger &
SoSFTrigger::operator = (const SoSFTrigger & /* field */)
{
  assert(0 && "FIXME: not implemented yet");
  return *this;
}

/*!
  FIXME: write function documentation
*/
void
SoSFTrigger::setValue(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoSFTrigger::getValue(void) const
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoSFTrigger::touch(void)
{
  this->startNotify();
}

/*!
  FIXME: write function documentation
*/
int
SoSFTrigger::operator == (const SoSFTrigger & /* trigger */) const
{
  assert(0 && "FIXME: not implemented yet");
  return -1;
}

/*!
  FIXME: write function documentation
*/
int
SoSFTrigger::operator != (const SoSFTrigger & /* trigger */) const
{
  assert(0 && "FIXME: not implemented yet");
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoSFTrigger::startNotify(void)
{
  SoNotList list;

  this->notify(&list);
}

/*!
  FIXME: write function documentation
*/
void
SoSFTrigger::notify(SoNotList * list)
{
  inherited::notify(list);

  //FIXME: What here? kintel
  //  this->touch();
}

/*!
  Constructor.
*/
SoSFTrigger::SoSFTrigger(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  Destructor.
*/
SoSFTrigger::~SoSFTrigger(void)
{
  assert(0 && "FIXME: not implemented yet");
}

SbBool
SoSFTrigger::readValue(SoInput * /* in */)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

void
SoSFTrigger::writeValue(SoOutput * /* out */) const
{
  assert(0 && "FIXME: not implemented yet");
}
