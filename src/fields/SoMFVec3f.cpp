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
  \class SoMFVec3f SoMFVec3f.h Inventor/fields/SoMFVec3f.h
  \brief The SoMFVec3f class is a container for SbVec3f vectors.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store an array of vectors with three elements.

  \sa SoSFVec3f

*/

#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE(SoMFVec3f, SbVec3f, const SbVec3f &);


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
  Set \a num vector array elements from \a xyz, starting at index
  \a start.
*/
void
SoMFVec3f::setValues(const int start, const int num, const float xyz[][3])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->set1Value(start+i, SbVec3f(xyz[i]));
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
  if (xyz == NULL) this->allocValues(0);
  else this->setValue(SbVec3f(xyz));
}

void
SoMFVec3f::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFVec3f::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFVec3f *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFVec3f::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
