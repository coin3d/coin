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
  \class SoSFRotation SoSFRotation.h Inventor/fields/SoSFRotation.h
  \brief The SoSFRotation class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/SbVec3f.h>

#include <Inventor/fields/SoSFMatrix.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFRotation.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32



SO_SFIELD_SOURCE(SoSFRotation, SbRotation, const SbRotation &);


/*!
  Does initialization common for all objects of the
  SoSFRotation class. This includes setting up the
  type system, among other things.
*/
void
SoSFRotation::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFRotation);
}

SbBool
SoSFRotation::readValue(SoInput * in)
{
  SbVec3f axis;
  float angle;
  SbBool result = (in->read(axis[0]) &&
                   in->read(axis[1]) &&
                   in->read(axis[2]) &&
                   in->read(angle));
  this->setValue(axis, angle);
  return result;
}

void
SoSFRotation::writeValue(SoOutput * out) const
{
  SbVec3f axis;
  float angle;

  this->getValue(axis, angle);

  // Handle invalid rotation specifications.
  if (axis.length() == 0.0f) {
    axis.setValue(0.0f, 0.0f, 1.0f);
    angle = 0.0f;
  }

  out->write(axis[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(axis[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(axis[2]);
  if(!out->isBinary()) out->write("  ");
  out->write(angle);
}

/*!
  FIXME: write function documentation
*/
void
SoSFRotation::getValue(SbVec3f & axis, float & angle) const
{
  evaluate();
  this->value.getValue(axis, angle);
}

/*!
  FIXME: write function documentation
*/
void
SoSFRotation::setValue(const float q0, const float q1,
                        const float q2, const float q3)
{
  this->setValue(SbRotation(q0, q1, q2, q3));
}

/*!
  FIXME: write function documentation
*/
void
SoSFRotation::setValue(const float q[4])
{
  this->setValue(SbRotation(q));
}

/*!
  FIXME: write function documentation
*/
void
SoSFRotation::setValue(const SbVec3f & axis, const float angle)
{
  this->setValue(SbRotation(axis, angle));
}

void
SoSFRotation::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFMatrix::getClassTypeId()) {
    SbMatrix mat;
    mat.setRotate(this->getValue());
    ((SoSFMatrix *)dest)->setValue(mat);
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    SbVec3f vec;
    float rad;
    this->getValue().getValue(vec,rad);
    ostr << vec[0] << " " << vec[1] << " " << vec[2] << "  " <<
      rad << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
  else if (dest->getTypeId()==SoMFRotation::getClassTypeId()) {
    ((SoMFRotation *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFRotation::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
