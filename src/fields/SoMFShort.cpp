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
  \class SoMFShort SoMFShort.h Inventor/fields/SoMFShort.h
  \brief The SoMFShort class is a container for short integer values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a group of multiple short integer values.

  \sa SoSFShort

*/

#include <Inventor/fields/SoMFShort.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/fields/SoSFShort.h>
#include <Inventor/fields/SoSFString.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFShort, short, short);


// Override from parent class.
void
SoMFShort::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFShort);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFShort class.
extern SbBool sosfshort_read_value(SoInput * in, short & val);
extern void sosfshort_write_value(SoOutput * out, const short val);

SbBool
SoMFShort::read1Value(SoInput * in, int idx)
{
  short val;
  if (!sosfshort_read_value(in, val)) return FALSE;
  this->set1Value(idx, val);
  return TRUE;
}

void
SoMFShort::write1Value(SoOutput * out, int idx) const
{
  sosfshort_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


// Store more than the default single value on each line for ASCII
// format export.
int
SoMFShort::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFShort::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFShort::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFShort *)dest)->setValue((*this)[0]);
  }
#if 0 // OBSOLETED: don't use libstdc++ stuff. 20000219 mortene
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    const int num=this->getNum();
    ostrstream ostr;
    if (num!=1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      ostr << (*this)[i];
      if (i<num-1) ostr << ", ";
    }
    if (num!=1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // OBSOLETED
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFRotation::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
