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
  \class SoSFVec3f SoSFVec3f.h Inventor/fields/SoSFVec3f.h
  \brief The SoSFVec3f class is a container for an SbVec3f vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with three elements.

  \sa SoMFVec3f

*/

#include <Inventor/fields/SoSFVec3f.h>
#include <SoSubFieldPrivate.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFVec3f, SbVec3f, const SbVec3f &);

// Override from parent class.
void
SoSFVec3f::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec3f);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read integer value from input stream, return TRUE if
// successful. Also used from SoMFVec3f class.
SbBool
sosfvec3f_read_value(SoInput * in, SbVec3f & v)
{
  if (!in->read(v[0]) || !in->read(v[1]) || !in->read(v[2])) {
    SoReadError::post(in, "Couldn't read vector");
    return FALSE;
  }
  return TRUE;
}

SbBool
SoSFVec3f::readValue(SoInput * in)
{
  SbVec3f v;
  if (!sosfvec3f_read_value(in, v)) return FALSE;
  this->setValue(v);
  return TRUE;
}

// Write integer value to output stream. Also used from SoMFVec3f
// class.
void
sosfvec3f_write_value(SoOutput * out, const SbVec3f & v)
{
  out->write(v[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[2]);
}

void
SoSFVec3f::writeValue(SoOutput * out) const
{
  sosfvec3f_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

/*!
  Set value of vector.
*/
void
SoSFVec3f::setValue(const float x, const float y, const float z)
{
  this->setValue(SbVec3f(x, y, z));
}

/*!
  Set value of vector.
*/
void
SoSFVec3f::setValue(const float xyz[3])
{
  this->setValue(SbVec3f(xyz));
}
