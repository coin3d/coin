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
