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
  \class SoSFColor SoSFColor.h Inventor/fields/SoSFColor.h
  \brief The SoSFColor class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <math.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoSFString.h>
#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFColor, SbColor, const SbColor &);


/*!
  Does initialization common for all objects of the
  SoSFColor class. This includes setting up the
  type system, among other things.
*/
void
SoSFColor::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFColor);
}

SbBool
SoSFColor::readValue(SoInput * in)
{
  return
    in->read(this->value[0]) &&
    in->read(this->value[1]) &&
    in->read(this->value[2]);
}

void
SoSFColor::writeValue(SoOutput * out) const
{
  SbColor c = this->getValue();

  out->write(c[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(c[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(c[2]);
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::setValue(const SbVec3f & vec)
{
  value.setValue(vec[0], vec[1], vec[2]);
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::setValue(const float red, const float green, const float blue)
{
  value.setValue(red, green, blue);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::setValue(const float rgb[3])
{
  value.setValue(rgb);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::setHSVValue(const float h, const float s, const float v)
{
  value.setHSVValue(h, s, v);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoSFColor::setHSVValue(const float hsv[3])
{
  value.setHSVValue(hsv);
  this->valueChanged();
}

void
SoSFColor::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFVec3f::getClassTypeId()) {
    ((SoSFVec3f *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    ostr << this->getValue()[0] << " " <<
      this->getValue()[1] << " " <<
      this->getValue()[2] << '\0';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
  else if (dest->getTypeId()==SoMFColor::getClassTypeId()) {
    ((SoMFColor *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFColor::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
