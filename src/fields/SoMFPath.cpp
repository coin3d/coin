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
  \class SoMFPath SoMFPath.h Inventor/fields/SoMFPath.h
  \brief The SoMFPath class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFPath.h>
#include <Inventor/fields/SoSFPath.h>
#include <Inventor/SbName.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFPath, SoPath *, SoPath *);


/*!
  Does initialization common for all objects of the
  SoMFPath class. This includes setting up the
  type system, among other things.
*/
void
SoMFPath::initClass(void)
{
  SO_MFIELD_INIT_CLASS(SoMFPath, inherited);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFPath::read1Value(SoInput * /* in */, int /* idx */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::write1Value(SoOutput * /* out */, int /* idx */) const
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::notify(SoNotList * /* list */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::fixCopy(SbBool /* copyConnections */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFPath::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

void
SoMFPath::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFPath::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFPath *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFPath::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
