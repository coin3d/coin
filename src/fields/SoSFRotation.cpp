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
  \class SoSFRotation SoSFRotation.h Inventor/fields/SoSFRotation.h
  \brief The SoSFRotation class is a container for an SbRotation.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single rotation definition.

  Fields of this type stores their value to file as a rotation axis
  vector plus a rotation angle: "axis0 axis1 axis2 angle".

  \sa SoMFRotation
*/

#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/errors/SoReadError.h>

#include <Inventor/fields/SoSFMatrix.h>
#include <Inventor/fields/SoMFRotation.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFRotation, SbRotation, const SbRotation &);


// Override from parent.
void
SoSFRotation::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFRotation);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read rotation value from input stream, return TRUE if
// successful. Also used from SoMFRotation class.
SbBool
sosfrotation_read_value(SoInput * in, SbRotation & r)
{
  SbVec3f axis;
  float angle;
  if (!(in->read(axis[0]) && in->read(axis[1]) && in->read(axis[2]) &&
        in->read(angle))) {
    SoReadError::post(in, "Couldn't read values");
    return FALSE;
  }

  r.setValue(axis, angle);
  return TRUE;
}

SbBool
SoSFRotation::readValue(SoInput * in)
{
  SbRotation r;
  if (!sosfrotation_read_value(in, r)) return FALSE;
  this->setValue(r);
  return TRUE;
}

// Write SbRotation to output stream. Also used from SoMFRotation
// class.
void
sosfrotation_write_value(SoOutput * out, const SbRotation & r)
{
  SbVec3f axis;
  float angle;
  r.getValue(axis, angle);

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

void
SoSFRotation::writeValue(SoOutput * out) const
{
  sosfrotation_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Return value of rotation as an \a axis and an \a angle around this
  \a axis.
*/
void
SoSFRotation::getValue(SbVec3f & axis, float & angle) const
{
  this->getValue().getValue(axis, angle);
}

/*!
  Set the rotation from a set of quaternion values.
*/
void
SoSFRotation::setValue(const float q0, const float q1,
                       const float q2, const float q3)
{
  this->setValue(SbRotation(q0, q1, q2, q3));
}

/*!
  Set the rotation from a set of quaternion values.
*/
void
SoSFRotation::setValue(const float q[4])
{
  this->setValue(SbRotation(q));
}

/*!
  Set the rotation from an \a axis and an \a angle around
  this \a axis.
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
#if 0 // OBSOLETED: don't use libstdc++ stuff. 20000219 mortene
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    SbVec3f vec;
    float rad;
    this->getValue().getValue(vec,rad);
    ostr << vec[0] << " " << vec[1] << " " << vec[2] << "  " <<
      rad << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // OBSOLETED
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
