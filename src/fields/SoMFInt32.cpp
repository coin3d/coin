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
  \class SoMFInt32 SoMFInt32.h Inventor/fields/SoMFInt32.h
  \brief The SoMFInt32 class is a container for 32-bit integer values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a group of multiple 32-bit integer values.

  \sa SoSFInt32
*/

#include <Inventor/fields/SoMFInt32.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFString.h>


SO_MFIELD_SOURCE_MALLOC(SoMFInt32, int32_t, int32_t);


// Override from parent.
void
SoMFInt32::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFInt32);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFInt32 class.
extern SbBool sosfint32_read_value(SoInput * in, int32_t & val);
extern void sosfint32_write_value(SoOutput * out, const int32_t val);

SbBool
SoMFInt32::read1Value(SoInput * in, int idx)
{
  int32_t val;
  if (!sosfint32_read_value(in, val)) return FALSE;
  this->set1Value(idx, val);
  return TRUE;
}

void
SoMFInt32::write1Value(SoOutput * out, int idx) const
{
  sosfint32_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


// Store more than the default single value on each line for ASCII
// format export.
int
SoMFInt32::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFInt32::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFInt32::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFInt32 *)dest)->setValue((*this)[0]);
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
    SoDebugError::post("SoMFInt32::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
