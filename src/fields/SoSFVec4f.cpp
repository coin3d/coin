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
  \class SoSFVec4f SoSFVec4f.h Inventor/fields/SoSFVec4f.h
  \brief The SoSFVec4f class is a container for an SbVec4f vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with four elements.

  \sa SoMFVec4f

*/

#include <Inventor/fields/SoSFVec4f.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFVec4f, SbVec4f, const SbVec4f &);


// Override from parent class.
void
SoSFVec4f::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec4f);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read integer value from input stream, return TRUE if
// successful. Also used from SoMFVec4f class.
SbBool
sosfvec4f_read_value(SoInput * in, SbVec4f & v)
{
  if (!in->read(v[0]) || !in->read(v[1]) || !in->read(v[2]) || !in->read(v[3])) {
    SoReadError::post(in, "Couldn't read vector");
    return FALSE;
  }
  return TRUE;
}

SbBool
SoSFVec4f::readValue(SoInput * in)
{
  SbVec4f v;
  if (!sosfvec4f_read_value(in, v)) return FALSE;
  this->setValue(v);
  return TRUE;
}

// Write integer value to output stream. Also used from SoMFVec4f
// class.
void
sosfvec4f_write_value(SoOutput * out, const SbVec4f & v)
{
  out->write(v[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[2]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[3]);
}

void
SoSFVec4f::writeValue(SoOutput * out) const
{
  sosfvec4f_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set value of vector.
*/
void
SoSFVec4f::setValue(const float x, const float y, const float z, const float w)
{
  this->setValue(SbVec4f(x, y, z, w));
}

/*!
  Set value of vector.
*/
void
SoSFVec4f::setValue(const float xyzw[4])
{
  this->setValue(SbVec4f(xyzw));
}
