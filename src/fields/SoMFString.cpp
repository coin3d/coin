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
  \class SoMFString SoMFString.h Inventor/fields/SoMFString.h
  \brief The SoMFString class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE(SoMFString, SbString, const SbString &);


/*!
  Does initialization common for all objects of the
  SoMFString class. This includes setting up the
  type system, among other things.
*/
void
SoMFString::initClass(void)
{
  SO_MFIELD_INIT_CLASS(SoMFString, inherited);
}

SbBool
SoMFString::read1Value(SoInput * in, int idx)
{
  SoSFString sfstring;
  SbBool result;
  if ((result = sfstring.readValue(in)))
    this->set1Value(idx, sfstring.getValue());
  return result;
}

void
SoMFString::write1Value(SoOutput * out, int idx) const
{
  SoSFString sfstring;
  sfstring.setValue((*this)[idx]);
  sfstring.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::setValues(const int start, const int num,
                     const char * const strings [])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for (int i=0;i<num;i++) this->set1Value(i+start,SbString(strings[i]));
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::setValue(const char * const string)
{
  this->setValue(SbString(string));
}

/*!
  FIXME: write function documentation
*/
void
SoMFString::deleteText(const int /* fromLine */, const int /* fromChar */,
                        const int /* toLine */, const int /* toChar */)
{
  COIN_STUB();
}

void
SoMFString::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFString *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFString::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
