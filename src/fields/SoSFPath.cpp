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
  \class SoSFPath SoSFPath.h Inventor/fields/SoSFPath.h
  \brief The SoSFPath class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFPath.h>
#include <Inventor/fields/SoMFPath.h>
#include <Inventor/SbName.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFPath, SoPath *, SoPath *);


/*!
  Does initialization common for all objects of the
  SoSFPath class. This includes setting up the
  type system, among other things.
*/
void
SoSFPath::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFPath);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFPath::readValue(SoInput * /* in */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFPath::notify(SoNotList * /* list */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoSFPath::fixCopy(SbBool /* copyConnections */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFPath::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFPath::writeValue(SoOutput * /* out */) const
{
  COIN_STUB();
}

void
SoSFPath::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoMFPath::getClassTypeId()) {
    ((SoMFPath *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFPath::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
