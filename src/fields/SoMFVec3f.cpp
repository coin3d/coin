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
  \class SoMFVec3f SoMFVec3f.h Inventor/fields/SoMFVec3f.h
  \brief The SoMFVec3f class is a container for SbVec3f vectors.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store an array of vectors with three elements.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SoSFVec3f

*/

#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSubFieldP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_MFIELD_SOURCE(SoMFVec3f, SbVec3f, const SbVec3f &);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec3f, SbVec3f, SbVec3f);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec3f, SbVec3f, float);

// Override from parent class.
void
SoMFVec3f::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFVec3f);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFVec3f class.
extern SbBool sosfvec3f_read_value(SoInput * in, SbVec3f & v);
extern void sosfvec3f_write_value(SoOutput * out, const SbVec3f & v);

SbBool
SoMFVec3f::read1Value(SoInput * in, int idx)
{
  SbVec3f v;
  if (!sosfvec3f_read_value(in, v)) return FALSE;
  this->set1Value(idx, v);
  return TRUE;
}

void
SoMFVec3f::write1Value(SoOutput * out, int idx) const
{
  sosfvec3f_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set \a num vector array elements from the \a xyz array, reading from
  index 0 in the given float-triplet vector array and storing values
  from index \a start in this SoMFVec3f's array.

  If \a start + \a num is larger than the current number of elements
  in the SoMFVec3f field, this method will automatically expand the
  field to accommodate all given float-vector element values.
*/
void
SoMFVec3f::setValues(const int start, const int num, const float xyz[][3])
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[start+i] = SbVec3f(xyz[i]);
  this->valueChanged();
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec3f::set1Value(const int idx,
                     const float x, const float y, const float z)
{
  this->set1Value(idx, SbVec3f(x, y, z));
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec3f::set1Value(const int idx, const float xyz[3])
{
  this->set1Value(idx, SbVec3f(xyz));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec3f::setValue(const float x, const float y, const float z)
{
  this->setValue(SbVec3f(x, y, z));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec3f::setValue(const float xyz[3])
{
  if (xyz == NULL) this->setNum(0);
  else this->setValue(SbVec3f(xyz));
}
