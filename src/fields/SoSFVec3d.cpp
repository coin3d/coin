/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoSFVec3d SoSFVec3d.h Inventor/fields/SoSFVec3d.h
  \brief The SoSFVec3d class is a container for an SbVec3d vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with three elements.

  \sa SoMFVec3f

*/

#include <Inventor/fields/SoSFVec3d.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFVec3d, SbVec3d, const SbVec3d &);

// Override from parent class.
void
SoSFVec3d::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec3d);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read integer value from input stream, return TRUE if
// successful. Also used from SoMFVec3f class.
SbBool
sosfvec3d_read_value(SoInput * in, SbVec3d & v)
{
  if (!in->read(v[0]) || !in->read(v[1]) || !in->read(v[2])) {
    SoReadError::post(in, "Couldn't read vector");
    return FALSE;
  }
  return TRUE;
}

SbBool
SoSFVec3d::readValue(SoInput * in)
{
  SbVec3d v;
  if (!sosfvec3d_read_value(in, v)) return FALSE;
  this->setValue(v);
  return TRUE;
}

// Write integer value to output stream. Also used from SoMFVec3f
// class.
void
sosfvec3d_write_value(SoOutput * out, const SbVec3d & v)
{
  out->write(v[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[2]);
}

void
SoSFVec3d::writeValue(SoOutput * out) const
{
  sosfvec3d_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

/*!
  Set value of vector.
*/
void
SoSFVec3d::setValue(const double x, const double y, const double z)
{
  this->setValue(SbVec3d(x, y, z));
}

/*!
  Set value of vector.
*/
void
SoSFVec3d::setValue(const double xyz[3])
{
  this->setValue(SbVec3d(xyz));
}
