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
  \class SoSFPlane SoSFPlane.h Inventor/fields/SoSFPlane.h
  \brief The SoSFPlane class is a container for an SbPlane value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single definition of a 3D plane.

  Fields of this type stores their value to file as a normalvector
  plus an offset from origo: "v0 v1 v2 offset".

  \sa SoMFPlane

*/

#include <Inventor/fields/SoSFPlane.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoReadError.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFPlane, SbPlane, const SbPlane &);


// Override from parent class.
void
SoSFPlane::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFPlane);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read SbPlane from input stream, return TRUE if successful. Also
// used from SoMFPlane class.
SbBool
sosfplane_read_value(SoInput * in, SbPlane & p)
{
  SbVec3f normal;
  float offset;
  if (!(in->read(normal[0]) && in->read(normal[1]) && in->read(normal[2]) &&
        in->read(offset))) {
    SoReadError::post(in, "Couldn't read values");
    return FALSE;
  }

  p = SbPlane(normal, offset);
  return TRUE;
}

SbBool
SoSFPlane::readValue(SoInput * in)
{
  SbPlane p;
  if (!sosfplane_read_value(in, p)) return FALSE;
  this->setValue(p);
  return TRUE;
}

// Write SbPlane value to output stream. Also used from SoMFPlane
// class.
void
sosfplane_write_value(SoOutput * out, const SbPlane & p)
{
  out->write(p.getNormal()[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(p.getNormal()[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(p.getNormal()[2]);
  if (!out->isBinary()) out->write("  ");
  out->write(p.getDistanceFromOrigin());
}

void
SoSFPlane::writeValue(SoOutput * out) const
{
  sosfplane_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS
