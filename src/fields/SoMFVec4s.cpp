/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoMFVec4s SoMFVec4s.h Inventor/fields/SoMFVec4s.h
  \brief The SoMFVec4s class is a container for SbVec4s vectors.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store an array of vectors with four elements.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SbVec4s, SoSFVec4s
  \COIN_CLASS_EXTENSION
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoMFVec4s.h>

#include <cassert>

#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_MFIELD_SOURCE(SoMFVec4s, SbVec4s, const SbVec4s &);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec4s, SbVec4s, SbVec4s);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec4s, SbVec4s, short);

// *************************************************************************

// Override from parent class.
void
SoMFVec4s::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFVec4s);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFVec4s::read1Value(SoInput * in, int idx)
{
  assert(idx < this->maxNum);
  return
    in->read(this->values[idx][0]) &&
    in->read(this->values[idx][1]) &&
    in->read(this->values[idx][2]) &&
    in->read(this->values[idx][3]);
}

void
SoMFVec4s::write1Value(SoOutput * out, int idx) const
{
  sosfvec4s_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set \a num vector array elements from \a xyzw, starting at index
  \a start.
*/
void
SoMFVec4s::setValues(int start, int numarg, const short xyzw[][4])
{
  if(start+numarg > this->maxNum) this->allocValues(start+numarg);
  else if(start+numarg > this->num) this->num = start+numarg;

  for(int i=0; i < numarg; i++) this->values[i+start].setValue(xyzw[i]);
  this->valueChanged();
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec4s::set1Value(int idx, short x, short y, short z, short w)
{
  this->set1Value(idx, SbVec4s(x, y, z, w));
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec4s::set1Value(int idx, const short xyzw[4])
{
  this->set1Value(idx, SbVec4s(xyzw));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec4s::setValue(short x, short y, short z, short w)
{
  this->setValue(SbVec4s(x,y,z,w));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec4s::setValue(const short xyzw[4])
{
  if (xyzw == NULL) this->setNum(0);
  else this->setValue(SbVec4s(xyzw));
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoMFVec4s field;
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
  BOOST_CHECK_EQUAL(field.getNum(), 0);
}

#endif // COIN_TEST_SUITE
