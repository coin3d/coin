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
  \class SoMFTime SoMFTime.h Inventor/fields/SoMFTime.h
  \brief The SoMFTime class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFTime.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFTime, SbTime, const SbTime &);


/*!
  Does initialization common for all objects of the
  SoMFTime class. This includes setting up the
  type system, among other things.
*/
void
SoMFTime::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFTime);
}

SbBool
SoMFTime::read1Value(SoInput * in, int idx)
{
  SoSFTime sftime;
  SbBool result;
  if ((result = sftime.readValue(in))) this->set1Value(idx, sftime.getValue());
  return result;
}

void
SoMFTime::write1Value(SoOutput * out, int idx) const
{
  SoSFTime sftime;
  sftime.setValue((*this)[idx]);
  sftime.writeValue(out);
}

void
SoMFTime::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFTime::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFTime *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFTime::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
