/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoSFBool SoSFBool.h Inventor/fields/SoSFBool.h
  \brief The SoSFBool class is a container for an SbBool value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a boolean on/off or TRUE/FALSE value, like for
  instance as the "on" field of the SoPointLight, SoSpotLight and
  SoDirectionalLight node classes.


  \sa SoMFBool
*/

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFBool, SbBool, SbBool);

// Override from parent.
void
SoSFBool::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFBool);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read boolean value from input stream, return TRUE if
// successful. Also used from SoMFBool class.
SbBool
sosfbool_read_value(SoInput * in, SbBool & val)
{
  // accept 0 or 1
  if (in->read(val)) {
    if (val != 0 && val != 1) {
      SoReadError::post(in, "Illegal value for field: %d (must be 0 or 1)",
                        val);
      return FALSE;
    }
    return TRUE;
  }

  if (in->isBinary()) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }

  // read TRUE/FALSE keyword

  SbName n;
  if (!in->read(n, TRUE)) {
    SoReadError::post(in, "Couldn't read field value");
    return FALSE;
  }

  if (n == "TRUE") {
    val = TRUE;
    return TRUE;
  }

  if (n == "FALSE") {
    val = FALSE;
    return TRUE;
  }

  SoReadError::post(in,
                    "Invalid value \"%s\" for field (must be TRUE or FALSE)",
                    n.getString());
  return FALSE;
}

SbBool
SoSFBool::readValue(SoInput * in)
{
  SbBool val;
  if (!sosfbool_read_value(in, val)) return FALSE;
  this->setValue(val);
  return TRUE;
}

// Write boolean value to output stream. Also used from SoMFBool
// class.
void
sosfbool_write_value(SoOutput * out, SbBool val)
{
  if (out->isBinary()) out->write((unsigned int)(val ? 1 : 0));
  else out->write(val ? "TRUE" : "FALSE");
}

void
SoSFBool::writeValue(SoOutput * out) const
{
  sosfbool_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS
