/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoSFTime SoSFTime.h Inventor/fields/SoSFTime.h
  \brief The SoSFTime class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFTime.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFTime.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFTime, SbTime, const SbTime &);


/*!
  Does initialization common for all objects of the
  SoSFTime class. This includes setting up the
  type system, among other things.
*/
void
SoSFTime::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFTime);
}

SbBool
SoSFTime::readValue(SoInput * in)
{
  double val;
  if (!in->read(val)) return FALSE;
  this->value.setValue(val);
  return TRUE;
}

void
SoSFTime::writeValue(SoOutput * out) const
{
  out->write(this->getValue().getValue());
}

void
SoSFTime::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFFloat::getClassTypeId()) {
    ((SoSFFloat *)dest)->setValue(float(this->getValue().getValue()));
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    if (this->getValue().getValue()>31500000.0)
      ((SoSFString *)dest)->setValue(this->getValue().formatDate());
    else
      ((SoSFString *)dest)->setValue(this->getValue().format());
  }
  else if (dest->getTypeId()==SoMFTime::getClassTypeId()) {
    ((SoMFTime *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFTime::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
