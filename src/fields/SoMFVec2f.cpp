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
  \class SoMFVec2f SoMFVec2f.h Inventor/fields/SoMFVec2f.h
  \brief The SoMFVec2f class is a container for SbVec2f vectors.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store an array of vectors with two elements.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SoSFVec2f

*/

#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoSubFieldP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_MFIELD_SOURCE(SoMFVec2f, SbVec2f, const SbVec2f &);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec2f, SbVec2f, SbVec2f);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec2f, SbVec2f, float);


// Override from parent class.
void
SoMFVec2f::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFVec2f);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFVec2f class.
extern SbBool sosfvec2f_read_value(SoInput * in, SbVec2f & v);
extern void sosfvec2f_write_value(SoOutput * out, const SbVec2f & v);

SbBool
SoMFVec2f::read1Value(SoInput * in, int idx)
{
  SbVec2f v;
  if (!sosfvec2f_read_value(in, v)) return FALSE;
  this->set1Value(idx, v);
  return TRUE;
}

void
SoMFVec2f::write1Value(SoOutput * out, int idx) const
{
  sosfvec2f_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set \a num vector array elements from \a xy, starting at index
  \a start.
*/
void
SoMFVec2f::setValues(const int start, const int num, const float xy[][2])
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[start+i] = SbVec2f(xy[i]);
  this->valueChanged();
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec2f::set1Value(const int idx, const float x, const float y)
{
  this->set1Value(idx, SbVec2f(x, y));
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec2f::set1Value(const int idx, const float xy[2])
{
  this->set1Value(idx, SbVec2f(xy));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec2f::setValue(const float x, const float y)
{
  this->setValue(SbVec2f(x, y));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec2f::setValue(const float xy[2])
{
  if (xy == NULL) this->setNum(0);
  else this->setValue(SbVec2f(xy[0], xy[1]));
}
