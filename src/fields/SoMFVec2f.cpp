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
  \class SoMFVec2f SoMFVec2f.h Inventor/fields/SoMFVec2f.h
  \brief The SoMFVec2f class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoSFVec2f.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFVec2f, SbVec2f, const SbVec2f &);


/*!
  Does initialization common for all objects of the
  SoMFVec2f class. This includes setting up the
  type system, among other things.
*/
void
SoMFVec2f::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFVec2f);
}

SbBool
SoMFVec2f::read1Value(SoInput * in, int idx)
{
  SoSFVec2f sfvec2f;
  SbBool result;
  if ((result = sfvec2f.readValue(in)))
    this->set1Value(idx, sfvec2f.getValue());
  return result;
}

void
SoMFVec2f::write1Value(SoOutput * out, int idx) const
{
  SoSFVec2f sfvec2f;
  sfvec2f.setValue((*this)[idx]);
  sfvec2f.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::setValues(const int start, const int num, const float xy[][2])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->set1Value(start+i, SbVec2f(xy[i]));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::set1Value(const int idx, const float x, const float y)
{
  this->set1Value(idx, SbVec2f(x, y));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::set1Value(const int idx, const float xy[2])
{
  this->set1Value(idx, SbVec2f(xy));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::setValue(const float x, const float y)
{
  this->setValue(SbVec2f(x, y));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec2f::setValue(const float xy[2])
{
  if (xy == NULL) this->allocValues(0);
  else this->setValue(SbVec2f(xy[0], xy[1]));
}

void
SoMFVec2f::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFVec2f::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFVec2f *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFVec2f::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
