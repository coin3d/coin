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
  \class SoSFVec3f SoSFVec3f.h Inventor/fields/SoSFVec3f.h
  \brief The SoSFVec3f class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFString.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32


SO_SFIELD_SOURCE(SoSFVec3f, SbVec3f, const SbVec3f &);

/*!
  Does initialization common for all objects of the
  SoSFVec3f class. This includes setting up the
  type system, among other things.
*/
void
SoSFVec3f::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec3f);
}

SbBool
SoSFVec3f::readValue(SoInput * in)
{
  return (in->read(value[0]) && in->read(value[1]) && in->read(value[2]));
}

void
SoSFVec3f::writeValue(SoOutput * out) const
{
  SbVec3f v = this->getValue(); // evaluate

  out->write(v[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(v[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(v[2]);
}

/*!
  FIXME: write function documentation
*/
void
SoSFVec3f::setValue(const float x, const float y, const float z)
{
  this->setValue(SbVec3f(x, y, z));
}

/*!
  FIXME: write function documentation
*/
void
SoSFVec3f::setValue(const float xyz[3])
{
  this->setValue(SbVec3f(xyz));
}

void
SoSFVec3f::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFColor::getClassTypeId()) {
    ((SoSFColor *)dest)->setValue(this->getValue().getValue());
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    ostr << this->getValue()[0] << " " <<
      this->getValue()[1] << " " <<
      this->getValue()[2] << '\0';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
  else if (dest->getTypeId()==SoMFVec3f::getClassTypeId()) {
    ((SoMFVec3f *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFVec3f::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
