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
  \class SoMFPlane SoMFPlane.h Inventor/fields/SoMFPlane.h
  \brief The SoMFPlane class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFPlane.h>
#include <Inventor/fields/SoSFPlane.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <Inventor/fields/SoSFString.h>

#ifdef _WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif



SO_MFIELD_SOURCE_MALLOC(SoMFPlane, SbPlane, const SbPlane &);


/*!
  Does initialization common for all objects of the
  SoMFPlane class. This includes setting up the
  type system, among other things.
*/
void
SoMFPlane::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFPlane);
}

SbBool
SoMFPlane::read1Value(SoInput * in, int idx)
{
  SoSFPlane sfplane;
  SbBool result;
  if ((result = sfplane.readValue(in)))
    this->set1Value(idx, sfplane.getValue());
  return result;
}

void
SoMFPlane::write1Value(SoOutput * out, int idx) const
{
  SoSFPlane sfplane;
  sfplane.setValue((*this)[idx]);
  sfplane.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
int
SoMFPlane::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFPlane::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFPlane::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFPlane *)dest)->setValue((*this)[0]);
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    const SbPlane * plane;
    const int num=this->getNum();

    if (num>1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      plane=&(*this)[i];
      const SbVec3f * norm=&plane->getNormal();
      ostr << (*norm)[0] << " " << (*norm)[1] << " " << (*norm)[2] << "  " <<
        plane->getDistanceFromOrigin();
      if (i<num-1) ostr << ", ";
    }
    if (num>1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFPlane::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
