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
  \class SoSFBitMask SoSFBitMask.h Inventor/fields/SoSFBitMask.h
  \brief The SoSFBitMask class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/fields/SoMFBitMask.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>



SO_SFIELD_DERIVED_SOURCE(SoSFBitMask, not_used, not_used);


/*!
  Does initialization common for all objects of the
  SoSFBitMask class. This includes setting up the
  type system, among other things.
*/
void
SoSFBitMask::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFBitMask);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFBitMask::findEnumValue(const SbName & name, int & val)
{
  // Look through names table for one that matches
  for (int i = 0; i < numEnums; i++) {
    if (name == enumNames[i]) {
      val = enumValues[i];
      return TRUE;
    }
  }
  return FALSE;
}

SbBool
SoSFBitMask::readValue(SoInput * in)
{
  assert(this->enumValues != NULL);

  this->value = 0;
  SbName n;

  if (in->isBinary()) {
    while (TRUE) {
      if (!in->read(n, TRUE)) {
        SoReadError::post(in, "Couldn't read SoSFBitMask bitmask value");
        return FALSE;
      }
      if (n.getLength() == 0) break;

      int v;
      if (this->findEnumValue(n, v)) {
        this->value |= v;
      }
      else {
        SoReadError::post(in, "Unknown SoSFBitMask bit "
                          "mask value \"%s\"", n.getString());
        return FALSE;
      }
    }

    return TRUE;
  }


  // Read first character
  char c;
  if (!in->read(c)) return FALSE;

  // Check for parenthesized list of bitwise-or'ed flags
  if (c == '(') {
    // Read names separated by '|'
    while (TRUE) {
      if (in->read(n, TRUE) && !(!n)) {

        int v;
        if (this->findEnumValue(n, v)) {
          this->value |= v;
        }
        else {
          SoReadError::post(in, "Unknown SoSFBitMask bit "
                            "mask value \"%s\"", n.getString());
          return FALSE;
        }
      }

      if (!in->read(c)) {
        SoReadError::post(in, "EOF reached before '%c' "
                          "in SoSFBitMask value", ')');
        return FALSE;
      }

      if (c == ')') break;

      else if (c != '|') {
        SoReadError::post(in,
                          "Expected '|' or ')', got '%c' in SoSFBitMask value",
                          c);
        return FALSE;
      }
    }
  }
  else {
    in->putBack(c);

    // Read mnemonic value as a character string identifier
    if (!in->read(n, TRUE)) return FALSE;

    if (!this->findEnumValue(n, this->value)) {
      SoReadError::post(in, "Unknown SoSFBitMask bit "
                        "mask value \"%s\"", n.getString());
      return FALSE;
    }
  }

  return TRUE;
}

void
SoSFBitMask::writeValue(SoOutput * out) const
{
  SbBool paran = FALSE;
  int out_vals_written = 0;

  // FIXME: as enumValues for SoSFBitMasks can be OR'ed combinations
  // of other enumValues, we should try to write a minimal set of
  // flags by masking out flags with the highest number of bits
  // first. 19991110 mortene.

  int restval = this->value;
  int i = 0;
  while (restval) {
    if (i >= this->numEnums) break;
    if (this->enumValues[i] & restval) {
      restval &= ~this->enumValues[i];
      if (!out_vals_written && restval) {
        if (!out->isBinary()) out->write('(');
        paran = TRUE;
      }
      if (out_vals_written++ && !out->isBinary()) out->write(" | ");
      out->write((const char *)this->enumNames[i].getString());
    }

    i++;
  }
  if (paran && !out->isBinary()) out->write(')');
  if (!out_vals_written && !out->isBinary()) out->write("()");

  if (out->isBinary()) out->write(0x00000000);

#if COIN_DEBUG
  if (restval) {
    SoDebugError::post("SoSFBitMask::writeValue",
                       "invalid bitmask -- some bits \"lost\" (0x%x) "
                       "upon export",
                       restval);
  }
#endif // COIN_DEBUG
}

void
SoSFBitMask::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoMFBitMask::getClassTypeId()) {
    ((SoMFBitMask *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFBitMask::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
