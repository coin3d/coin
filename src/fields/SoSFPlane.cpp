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
  \class SoSFPlane SoSFPlane.h Inventor/fields/SoSFPlane.h
  \brief The SoSFPlane class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFPlane.h>
#include <Inventor/fields/SoMFPlane.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SoInput.h>
#include <Inventor/SbName.h>
#include <Inventor/fields/SoSFString.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32



SO_SFIELD_SOURCE(SoSFPlane, SbPlane, const SbPlane &);


/*!
  Does initialization common for all objects of the
  SoSFPlane class. This includes setting up the
  type system, among other things.
*/
void
SoSFPlane::initClass(void)
{
  SO_SFIELD_INIT_CLASS(SoSFPlane, inherited);
}

SbBool
SoSFPlane::readValue(SoInput * in)
{
  SbVec3f normal;
  float offset;
  SbBool result = (in->read(normal[0]) &&
                   in->read(normal[1]) &&
                   in->read(normal[2]) &&
                   in->read(offset));
  this->setValue(SbPlane(normal, offset));
  return result;
}

void
SoSFPlane::writeValue(SoOutput * out) const
{
  SbPlane p = this->getValue(); // implicit evaluate() call

  out->write(p.getNormal()[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(p.getNormal()[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(p.getNormal()[2]);
  if (!out->isBinary()) out->write("  ");
  out->write(p.getDistanceFromOrigin());
}

void
SoSFPlane::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoMFPlane::getClassTypeId()) {
    ((SoMFPlane *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    const SbPlane plane=this->getValue();
    const SbVec3f norm=plane.getNormal();

    ostr << norm[0] << " " << norm[1] << " " << norm[2] << "  " <<
      plane.getDistanceFromOrigin() << ends;

    ((SoSFString *)dest)->setValue(ostr.str());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFPlane::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
