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
  \class SoMFVec3f SoMFVec3f.h Inventor/fields/SoMFVec3f.h
  \brief The SoMFVec3f class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFVec3f, SbVec3f, const SbVec3f &);


/*!
  Does initialization common for all objects of the
  SoMFVec3f class. This includes setting up the
  type system, among other things.
*/
void
SoMFVec3f::initClass(void)
{
  SO_MFIELD_INIT_CLASS(SoMFVec3f, inherited);
}

SbBool
SoMFVec3f::read1Value(SoInput * in, int idx)
{
  SoSFVec3f sfvec3f;
  SbBool result;
  if ((result = sfvec3f.readValue(in)))
    this->set1Value(idx, sfvec3f.getValue());
  return result;
}

void
SoMFVec3f::write1Value(SoOutput * out, int idx) const
{
  SoSFVec3f sfvec3f;
  sfvec3f.setValue((*this)[idx]);
  sfvec3f.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec3f::setValues(const int start, const int num, const float xyz[][3])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->set1Value(start+i, SbVec3f(xyz[i]));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec3f::set1Value(const int idx,
                      const float x, const float y, const float z)
{
  this->set1Value(idx, SbVec3f(x, y, z));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec3f::set1Value(const int idx, const float xyz[3])
{
  this->set1Value(idx, SbVec3f(xyz));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec3f::setValue(const float x, const float y, const float z)
{
  this->setValue(SbVec3f(x, y, z));
}

/*!
  FIXME: write function documentation
*/
void
SoMFVec3f::setValue(const float xyz[3])
{
  if (xyz == NULL) this->allocValues(0);
  else this->setValue(SbVec3f(xyz));
}

void
SoMFVec3f::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFVec3f::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFVec3f *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFVec3f::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
