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
  \class SoSFBool SoSFBool.h Inventor/fields/SoSFBool.h
  \brief The SoSFBool class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SbName.h>

#include <assert.h>

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFUInt32.h>
#include <Inventor/fields/SoSFShort.h>
#include <Inventor/fields/SoSFUShort.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFBool.h>


SO_SFIELD_SOURCE(SoSFBool, SbBool, const SbBool);

/*!
  Does initialization common for all objects of the SoSFBool
  class. This includes setting up the type system, among other things.
*/
void
SoSFBool::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFBool);
}

SbBool
SoSFBool::readValue(SoInput * in)
{
  // accept 0 or 1
  if (in->read(this->value)) {
    if (this->value != 0 && this->value != 1) {
      SoReadError::post(in, "Illegal value for SoSFBool: %d "
                        "(must be 0 or 1)", this->value);
      return FALSE;
    }
    return TRUE;
  }

  if (in->isBinary()) {
    SoReadError::post(in, "Couldn't read field");
    return FALSE;
  }

  // read TRUE/FALSE keyword
  SbName n;
  if (!in->read(n, TRUE)) return FALSE;

  if (n == "TRUE") {
    this->value = TRUE;
    return TRUE;
  }

  if (n == "FALSE") {
    this->value = FALSE;
    return TRUE;
  }

  SoReadError::post(in, "Unknown value (\"%s\") for SoSFBool (must be TRUE or "
                    "FALSE)", n.getString());
  return FALSE;
}

void
SoSFBool::writeValue(SoOutput * out) const
{
  if (out->isBinary()) out->write((unsigned int)(this->getValue() ? 1 : 0));
  else out->write(this->getValue() ? "TRUE" : "FALSE");
}

void
SoSFBool::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFFloat::getClassTypeId()) {
    ((SoSFFloat *)dest)->setValue((float)this->getValue());
  }
  else if (dest->getTypeId()==SoSFInt32::getClassTypeId()) {
    ((SoSFInt32 *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFShort::getClassTypeId()) {
    ((SoSFShort *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFUInt32::getClassTypeId()) {
    ((SoSFUInt32 *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFUShort::getClassTypeId()) {
    ((SoSFUShort *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ((SoSFString *)dest)->setValue(this->getValue()?"TRUE":"FALSE");
  }
  else if (dest->getTypeId()==SoMFBool::getClassTypeId()) {
    ((SoMFBool *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFBool::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
