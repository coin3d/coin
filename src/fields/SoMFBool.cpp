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
  \class SoMFBool SoMFBool.h Inventor/fields/SoMFBool.h
  \brief The SoMFBool class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoSFBool.h>

#include <Inventor/fields/SoSFString.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <malloc.h>
#include <assert.h>

SO_MFIELD_SOURCE_MALLOC(SoMFBool, SbBool, SbBool);


/*!
  Does initialization common for all objects of the
  SoMFBool class. This includes setting up the
  type system, among other things.
*/
void
SoMFBool::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFBool);
}

SbBool
SoMFBool::read1Value(SoInput * in, int idx)
{
  SoSFBool sfbool;
  SbBool result;
  if ((result = sfbool.readValue(in))) this->set1Value(idx, sfbool.getValue());
  return result;
}

void
SoMFBool::write1Value(SoOutput * out, int idx) const
{
  SoSFBool sfbool;
  sfbool.setValue((*this)[idx]);
  sfbool.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
int
SoMFBool::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFBool::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFBool::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFBool *)dest)->setValue((*this)[0]);
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    const int num=this->getNum() - 1;
    SbString ostr( "" );
    if ( num != 0 ) ostr += "[ ";
    for ( int i = 0; i < num; i++ )
      ostr += ((*this)[i]) ? "TRUE, " : "FALSE, ";
    ostr += ((*this)[num]) ? "TRUE" : "FALSE";
    if ( num != 0 ) ostr += " ]";
    ((SoSFString *)dest)->setValue( ostr.getString() );
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFBool::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
