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
  \class SoMFVec4f SoMFVec4f.h Inventor/fields/SoMFVec4f.h
  \brief The SoMFVec4f class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFVec4f.h>
#include <Inventor/fields/SoSFVec4f.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFVec4f, SbVec4f, const SbVec4f &);


/*!
  Does initialization common for all objects of the
  SoMFVec4f class. This includes setting up the
  type system, among other things.
*/
void
SoMFVec4f::initClass(void)
{
  SO_MFIELD_INIT_CLASS(SoMFVec4f, inherited);
}

SbBool
SoMFVec4f::read1Value(SoInput * in, int idx)
{
  SoSFVec4f sfvec4f;
  SbBool result;
  if ((result = sfvec4f.readValue(in)))
    this->set1Value(idx, sfvec4f.getValue());
  return result;
}

void
SoMFVec4f::write1Value(SoOutput * out, int idx) const
{
  SoSFVec4f sfvec4f;
  sfvec4f.setValue((*this)[idx]);
  sfvec4f.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::setValues(const int start, const int num, const float xyzw[][4])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[i+start].setValue(xyzw[i]);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::set1Value(const int idx, const float x, const float y,
                      const float z, const float w)
{
  this->set1Value(idx, SbVec4f(x, y, z, w));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::set1Value(const int idx, const float xyzw[4])
{
  this->set1Value(idx, SbVec4f(xyzw));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::setValue(const float x, const float y,
                     const float z, const float w)
{
  this->setValue(SbVec4f(x,y,z,w));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec4f::setValue(const float xyzw[4])
{
  if (xyzw == NULL) this->allocValues(0);
  else this->setValue(SbVec4f(xyzw));
}

void
SoMFVec4f::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFVec4f::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFVec4f *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFVec4f::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
