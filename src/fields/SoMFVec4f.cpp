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
  \class SoMFVec4f SoMFVec4f.h Inventor/fields/SoMFVec4f.h
  \brief The SoMFVec4f class is a container for SbVec4f vectors.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store an array of vectors with four elements.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SoSFVec4f

*/

#include <Inventor/fields/SoMFVec4f.h>
#include <Inventor/fields/SoSubFieldP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_MFIELD_SOURCE(SoMFVec4f, SbVec4f, const SbVec4f &);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec4f, SbVec4f, SbVec4f);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec4f, SbVec4f, float);

// Override from parent class.
void
SoMFVec4f::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFVec4f);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFVec4f class.
extern SbBool sosfvec4f_read_value(SoInput * in, SbVec4f & v);
extern void sosfvec4f_write_value(SoOutput * out, const SbVec4f & v);

SbBool
SoMFVec4f::read1Value(SoInput * in, int idx)
{
  SbVec4f v;
  if (!sosfvec4f_read_value(in, v)) return FALSE;
  this->set1Value(idx, v);
  return TRUE;
}

void
SoMFVec4f::write1Value(SoOutput * out, int idx) const
{
  sosfvec4f_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set \a num vector array elements from \a xyzw, starting at index
  \a start.
*/
void
SoMFVec4f::setValues(const int start, const int num, const float xyzw[][4])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[i+start].setValue(xyzw[i]);
  this->valueChanged();
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec4f::set1Value(const int idx, const float x, const float y,
                     const float z, const float w)
{
  this->set1Value(idx, SbVec4f(x, y, z, w));
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec4f::set1Value(const int idx, const float xyzw[4])
{
  this->set1Value(idx, SbVec4f(xyzw));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec4f::setValue(const float x, const float y,
                    const float z, const float w)
{
  this->setValue(SbVec4f(x,y,z,w));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec4f::setValue(const float xyzw[4])
{
  if (xyzw == NULL) this->setNum(0);
  else this->setValue(SbVec4f(xyzw));
}
