/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoSFString SoSFString.h Inventor/fields/SoSFString.h
  \brief The SoSFString class is a container for an SbString.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single string.

  Fields of this type stores their value to file as a the string
  within quotes.

  \sa SoMFString

*/

// *************************************************************************

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSubFieldP.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

// FIXME: to support a hack below, should not really be
// necessary. 20040614 mortene.
#include <Inventor/nodes/SoNode.h>

// *************************************************************************

SO_SFIELD_SOURCE(SoSFString, SbString, const SbString &);

// *************************************************************************

// Override from parent class.
void
SoSFString::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFString);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFString::readValue(SoInput * in)
{
  return in->read(this->value);
}

// Write SbString value to output stream. Also used from SoMFString class.
void
sosfstring_write_value(const SoField * f, SoOutput * out, const SbString & val)
{
  // VRML97 needs backslashes themselves to be backslash-quoted (like
  // in e.g. C strings), which is taken care of here instead of
  // downstream, as this is the last place we can find out whether or
  // not we're writing a VRML97 node.
  const SoFieldContainer * fc = f->getContainer();
  if (fc && fc->isOfType(SoNode::getClassTypeId()) &&
      (((SoNode *)fc)->getNodeType() & SoNode::VRML2)) {
    // FIXME: SbString should have had a replaceAll() method, so we
    // wouldn't have to spell out the iteration loop below. 20040614 mortene.
    SbString ws;
    for (int i = 0; i < val.getLength(); i++) {
      if (val[i] == '\\') { ws += '\\'; }
      ws += val[i];
    }
    out->write(ws);
  }
  else { // *Not* a VRML97 node, so write backslashes verbatim, as
         // dictated by the Inventor file format:
    out->write(val);
  }
}

void
SoSFString::writeValue(SoOutput * out) const
{
  sosfstring_write_value(this, out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set string field value from \a str.
*/
void
SoSFString::setValue(const char * str)
{
  this->setValue(SbString(str));
}

// *************************************************************************
