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
  \class SoSFVec2s SoSFVec2s.h Inventor/fields/SoSFVec2s.h
  \brief The SoSFVec2s class is a container for an SbVec2s vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with two elements.

  \since 2002-01-25

  \sa SoMFVec2s
*/

#include <Inventor/fields/SoSFVec2s.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFVec2s, SbVec2s, const SbVec2s &);


// Override from parent class.
void
SoSFVec2s::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec2s);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read integer value from input stream, return TRUE if
// successful. Also used from SoMFVec2s class.
SbBool
sosfvec2s_read_value(SoInput * in, SbVec2s & v)
{
  if (!in->read(v[0]) || !in->read(v[1])) {
    SoReadError::post(in, "Couldn't read vector");
    return FALSE;
  }
  return TRUE;
}

SbBool
SoSFVec2s::readValue(SoInput * in)
{
  SbVec2s v;
  if (!sosfvec2s_read_value(in, v)) return FALSE;
  this->setValue(v);
  return TRUE;
}

// Write integer value to output stream. Also used from SoMFVec2s
// class.
void
sosfvec2s_write_value(SoOutput * out, const SbVec2s & v)
{
  out->write(v[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[1]);
}

void
SoSFVec2s::writeValue(SoOutput * out) const
{
  sosfvec2s_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set value of vector.
*/
void
SoSFVec2s::setValue(const short x, const short y)
{
  this->setValue(SbVec2s(x, y));
}

/*!
  Set value of vector.
*/
void
SoSFVec2s::setValue(const short xy[2])
{
  this->setValue(SbVec2s(xy));
}
