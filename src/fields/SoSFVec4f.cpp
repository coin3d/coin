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
  \class SoSFVec4f SoSFVec4f.h Inventor/fields/SoSFVec4f.h
  \brief The SoSFVec4f class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFVec4f.h>
#include <Inventor/fields/SoMFVec4f.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFVec4f, SbVec4f, const SbVec4f &);


/*!
  Does initialization common for all objects of the SoSFVec4f
  class. This includes setting up the type system, among other things.
*/
void
SoSFVec4f::initClass(void)
{
  SO_SFIELD_INIT_CLASS(SoSFVec4f, inherited);
}

SbBool
SoSFVec4f::readValue(SoInput * in)
{
  return
    in->read(value[0]) && in->read(value[1]) &&
    in->read(value[2]) && in->read(value[3]);
}

void
SoSFVec4f::writeValue(SoOutput * out) const
{
  SbVec4f v = this->getValue(); // evaluate

  out->write(v[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(v[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(v[2]);
  if(!out->isBinary()) out->write(' ');
  out->write(v[3]);
}

/*!
  FIXME: write function documentation
*/
void
SoSFVec4f::setValue(const float x, const float y, const float z, const float w)
{
  this->setValue(SbVec4f(x, y, z, w));
}

/*!
  FIXME: write function documentation
*/
void
SoSFVec4f::setValue(const float xyzw[4])
{
  this->setValue(SbVec4f(xyzw));
}

void
SoSFVec4f::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoMFVec4f::getClassTypeId()) {
    ((SoMFVec4f *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFVec4f::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
