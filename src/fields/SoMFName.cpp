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
  \class SoMFName SoMFName.h Inventor/fields/SoMFName.h
  \brief The SoMFName class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoSFName.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <assert.h>

#include <Inventor/fields/SoSFString.h>
#ifdef _WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif



SO_MFIELD_SOURCE_MALLOC(SoMFName, SbName, SbName);


/*!
  Does initialization common for all objects of the
  SoMFName class. This includes setting up the
  type system, among other things.
*/
void
SoMFName::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFName);
}

SbBool
SoMFName::read1Value(SoInput * in, int idx)
{
  SoSFName sfname;
  SbBool result;
  if ((result = sfname.readValue(in))) this->set1Value(idx, sfname.getValue());
  return result;
}

void
SoMFName::write1Value(SoOutput * out, int idx) const
{
  SoSFName sfname;
  sfname.setValue((*this)[idx]);
  sfname.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFName::setValues(const int start, const int num,
                     const char * const strings [])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++)
    this->values[i+start] = SbName(strings[i]);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFName::setValue(const char * const string)
{
  this->setValue(SbName(string));
}

void
SoMFName::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFName::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFName *)dest)->setValue((*this)[0]);
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    const int num=this->getNum();
    if (num>1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      ostr << '"' << (*this)[i].getString() << '"';
      if (i<num-1) ostr << ", ";
    }
    if (num>1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFName::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
