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
  \class SoSFVec2f SoSFVec2f.h Inventor/fields/SoSFVec2f.h
  \brief The SoSFVec2f class is a container for an SbVec2f vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with two elements.

  \sa SoMFVec2f

*/

#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFVec2f, SbVec2f, const SbVec2f &);


// Override from parent class.
void
SoSFVec2f::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec2f);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read integer value from input stream, return TRUE if
// successful. Also used from SoMFVec2f class.
SbBool
sosfvec2f_read_value(SoInput * in, SbVec2f & v)
{
  if (!in->read(v[0]) || !in->read(v[1])) {
    SoReadError::post(in, "Couldn't read vector");
    return FALSE;
  }
  return TRUE;
}

SbBool
SoSFVec2f::readValue(SoInput * in)
{
  SbVec2f v;
  if (!sosfvec2f_read_value(in, v)) return FALSE;
  this->setValue(v);
  return TRUE;
}

// Write integer value to output stream. Also used from SoMFVec2f
// class.
void
sosfvec2f_write_value(SoOutput * out, const SbVec2f & v)
{
  out->write(v[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[1]);
}

void
SoSFVec2f::writeValue(SoOutput * out) const
{
  sosfvec2f_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set value of vector.
*/
void
SoSFVec2f::setValue(const float x, const float y)
{
  this->setValue(SbVec2f(x, y));
}

/*!
  Set value of vector.
*/
void
SoSFVec2f::setValue(const float xy[2])
{
  this->setValue(SbVec2f(xy));
}
