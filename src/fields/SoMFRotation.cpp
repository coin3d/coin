/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoMFRotation SoMFRotation.h Inventor/fields/SoMFRotation.h
  \brief The SoMFRotation class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/fields/SoSFRotation.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/SbVec3f.h>
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <Inventor/fields/SoSFString.h>

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32



SO_MFIELD_SOURCE_MALLOC(SoMFRotation, SbRotation, const SbRotation &);

/*!
  Does initialization common for all objects of the
  SoMFRotation class. This includes setting up the
  type system, among other things.
*/
void
SoMFRotation::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFRotation);
}

SbBool
SoMFRotation::read1Value(SoInput * in, int idx)
{
  SoSFRotation sfrotation;
  SbBool result;
  if ((result = sfrotation.readValue(in)))
    this->set1Value(idx, sfrotation.getValue());
  return result;
}

void
SoMFRotation::write1Value(SoOutput * out, int idx) const
{
  SoSFRotation sfrotation;
  sfrotation.setValue((*this)[idx]);
  sfrotation.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::setValues(const int start, const int num, const float q[][4])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[i+start].setValue(q[i]);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::set1Value(const int idx, const float q0, const float q1,
                         const float q2, const float q3)
{
  this->set1Value(idx,SbRotation(q0,q1,q2,q3));
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::set1Value(const int idx, const float q[4])
{
  this->set1Value(idx,SbRotation(q[0],q[1],q[2],q[3]));
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::set1Value(const int idx, const SbVec3f & axis, const float angle)
{
  this->set1Value(idx,SbRotation(axis,angle));
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::setValue(const float q0, const float q1, const float q2, const float q3)
{
  this->setValue(SbRotation(q0,q1,q2,q3));
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::setValue(const float q[4])
{
  this->setValue(SbRotation(q[0],q[1],q[2],q[3]));
}

/*!
  FIXME: write function documentation
*/
void
SoMFRotation::setValue(const SbVec3f & axis, const float angle)
{
  this->setValue(SbRotation(axis,angle));
}

void
SoMFRotation::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFRotation::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFRotation *)dest)->setValue((*this)[0]);
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    SbVec3f vec;
    float rad;
    const int num=this->getNum();

    if (num>1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      (*this)[i].getValue(vec,rad);
      ostr << vec[0] << " " << vec[1] << " " << vec[2] << "  " <<
        rad;
      if (i<num-1) ostr << ", ";
    }
    if (num>1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFRotation::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
