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
  \class SoMFColor SoMFColor.h Inventor/fields/SoMFColor.h
  \brief The SoMFColor class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoSFColor.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <assert.h>



SO_MFIELD_SOURCE_MALLOC(SoMFColor, SbColor, const SbColor &);


/*!
  Does initialization common for all objects of the
  SoMFColor class. This includes setting up the
  type system, among other things.
*/
void
SoMFColor::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFColor);
}

SbBool
SoMFColor::read1Value(SoInput * in, int idx)
{
  SoSFColor sfcolor;
  SbBool result;
  if ((result = sfcolor.readValue(in)))
    this->set1Value(idx, sfcolor.getValue());
  return result;
}

void
SoMFColor::write1Value(SoOutput * out, int idx) const
{
  SoSFColor sfcolor;
  sfcolor.setValue((*this)[idx]);
  sfcolor.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setValues(const int start, const int num, const float rgb[][3])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[i+start].setValue(rgb[i]);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setHSVValues(const int /* start */, const int /* num */,
                        const float /* hsv */[][3])
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setValue(const SbVec3f & vec)
{
  this->setValue(SbColor(vec));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setValue(const float r, const float g, const float b)
{
  this->setValue(SbColor(r, g, b));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setValue(const float rgb[3])
{
  this->setValue(SbColor(rgb));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setHSVValue(const float /* h */, const float /* s */, const float /* v */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setHSVValue(const float /* hsv */[3])
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1Value(const int idx, const SbVec3f & vec)
{
  this->set1Value(idx, SbColor(vec));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1Value(const int idx, const float r, const float g, const float b)
{
  this->set1Value(idx,SbColor(r,g,b));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1Value(const int idx, const float rgb[3])
{
  this->set1Value(idx,SbColor(rgb));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1HSVValue(const int /* idx */,
                        const float /* h */, const float /* s */, const float /* v */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1HSVValue(const int /* idx */, const float /* hsv */[3])
{
  COIN_STUB();
}

void
SoMFColor::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFColor::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFColor *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFColor::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
