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
  \class SoSFVec2f SoSFVec2f.h Inventor/fields/SoSFVec2f.h
  \brief The SoSFVec2f class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFVec2f, SbVec2f, const SbVec2f &);


/*!
  Does initialization common for all objects of the
  SoSFVec2f class. This includes setting up the
  type system, among other things.
*/
void
SoSFVec2f::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec2f);
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
  if (dest->getTypeId()==SoMFVec2f::getClassTypeId()) {
    ((SoMFVec2f *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFVec2f::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
