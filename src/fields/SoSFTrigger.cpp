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


SO_SFIELD_CONSTRUCTOR_SOURCE(SoSFTrigger);
// The following code is the internals of the
// SO_SFIELD_REQUIRED_SOURCE macro, with a minor modification
// (setValue() takes a void argument).
PRIVATE_TYPEID_SOURCE(SoSFTrigger);
PRIVATE_EQUALITY_SOURCE(SoSFTrigger);
const SoSFTrigger &
SoSFTrigger::operator=(const SoSFTrigger & field)
{
  this->setValue();
  return *this;
}


/*!
  Does initialization common for all objects of the
  SoSFTrigger class. This includes setting up the
  type system, among other things.
*/
void
SoSFTrigger::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFTrigger);
}

/*!
  FIXME: write function documentation
*/
void
SoSFTrigger::setValue(void)
{
  this->touch();
}

/*!
  FIXME: write function documentation
*/
void
SoSFTrigger::getValue(void) const
{
  // Does nothing.
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
SoSFTrigger::operator==(const SoSFTrigger & /* trigger */) const
{
  COIN_STUB();
  return -1;
}

/*!
  FIXME: write function documentation
*/
int
SoSFTrigger::operator!=(const SoSFTrigger & /* trigger */) const
{
  COIN_STUB();
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

SbBool
SoSFTrigger::readValue(SoInput * /* in */)
{
  COIN_STUB();
  return FALSE;
}

void
SoSFTrigger::writeValue(SoOutput * /* out */) const
{
  COIN_STUB();
}
