/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoSFBitMask SoSFBitMask.h Inventor/fields/SoSFBitMask.h
  \brief The SoSFBitMask class is a container for a set of bit flags.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store one particular set of bit flags from an enumerated
  set.

  A field of this type stores its value to file as the symbolic
  names, rather than the actual integer bitmask. If more than one
  bit is set in the value when exporting, the bits are written like
  this: "(BITNAME|BITNAME|BITNAME|...)".

  \sa SoMFBitMask

*/

#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>



SO_SFIELD_DERIVED_SOURCE(SoSFBitMask, not_used, not_used);

// Override from parent class.
void
SoSFBitMask::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFBitMask);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFBitMask::readValue(SoInput * in)
{
  // FIXME: in this case, perhaps we should rather accept numeric
  // values instead of demanding mnemonics? 20020630 mortene.
  if (!this->legalValuesSet) {
    SbName name;
    SoFieldContainer * container = this->getContainer();
    SbBool fname = container && container->getFieldName(this, name);
    SoReadError::post(in,
                      "no mappings available for SoSFBitMask field %s",
                      fname ? name.getString() : "");
    return FALSE;
  }

  if (in->isBinary()) {
    int bitmask = 0;
    while (TRUE) {
      SbName n;
      if (!in->read(n, TRUE)) {
        SoReadError::post(in, "Couldn't read SoSFBitMask bitmask value");
        return FALSE;
      }
      if (n.getLength() == 0) break;

      int v;
      if (!this->findEnumValue(n, v)) {
        SoReadError::post(in, "Unknown SoSFBitMask bit "
                          "mask value \"%s\"", n.getString());
        return FALSE;
      }

      bitmask |= v;
    }

    this->setValue(bitmask);
    return TRUE;
  }


  // Read first character
  char c;
  if (!in->read(c)) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }

  // Check for parenthesized list of bitwise-or'ed flags
  if (c == '(') {
    int bitmask = 0;

    // Read names separated by '|'
    while (TRUE) {
      SbName n;
      if (in->read(n, TRUE) && !(!n)) {
        int v;
        if (!this->findEnumValue(n, v)) {
          SoReadError::post(in, "Unknown SoSFBitMask bit "
                            "mask value \"%s\"", n.getString());
          return FALSE;
        }

        bitmask |= v;
      }

      if (!in->read(c)) {
        SoReadError::post(in, "EOF reached before ')' in SoSFBitMask value");
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

    this->setValue(bitmask);
  }
  else {
    in->putBack(c);

    // Read mnemonic value as a character string identifier
    SbName n;
    if (!in->read(n, TRUE)) {
      SoReadError::post(in, "Couldn't read SoSFBitMask bit name");
      return FALSE;
    }

    int v;
    if (!this->findEnumValue(n, v)) {
      SoReadError::post(in, "Unknown SoSFBitMask bit mask value \"%s\"",
                        n.getString());
      return FALSE;
    }

    this->setValue(v);
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

  int restval = this->getValue();
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

#endif // DOXYGEN_SKIP_THIS
