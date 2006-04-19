/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoSFBox3f SoSFBox3f.h Inventor/fields/SoSFBox3f.h
  \brief The SoSFBox3f class is a container for an SbBox3f vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a box.

  \COIN_CLASS_EXTENSION

  \since 20060419
*/

// *************************************************************************

#include <Inventor/fields/SoSFBox3f.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

// *************************************************************************

SO_SFIELD_SOURCE(SoSFBox3f, SbBox3f, SbBox3f);

// *************************************************************************

// Override from parent class.
void
SoSFBox3f::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFBox3f);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS


SbBool
SoSFBox3f::readValue(SoInput * in)
{
  float min[3];
  float max[3];
  if (!in->read(min[0]) || 
      !in->read(min[1]) || 
      !in->read(min[2]) || 
      !in->read(max[0]) || 
      !in->read(max[1]) || 
      !in->read(max[2])) {
    SoReadError::post(in, "Couldn't read SoSFBox3f");
    return FALSE;
  }
  this->setValue(min[0], min[1], min[2], max[0], max[1], max[2]);
  return TRUE;
}

void
SoSFBox3f::writeValue(SoOutput * out) const
{
  float min[3];
  float max[3];
  SbBox3f b = this->getValue();
  b.getBounds(min[0], min[1], min[2], max[0], max[1], max[2]);
  out->write(min[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(min[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(min[2]);
  if (!out->isBinary()) out->write(' ');
  out->write(max[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(max[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(max[2]);
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set value of vector.
*/
void
SoSFBox3f::setValue(float xmin, 
                    float ymin, 
                    float zmin, 
                    float xmax, 
                    float ymax, 
                    float zmax)
{
  this->setValue(SbBox3f(xmin, ymin, zmin, xmax, ymax, zmax));
}


/*!
  Set value of vector.
*/
void
SoSFBox3f::setValue(const SbVec3f & minvec, const SbVec3f & maxvec)
{
  this->setValue(SbBox3f(minvec, maxvec));
}


/*!
  Set value of vector.
*/
void
SoSFBox3f::getValue(SbBox3f &box)
{
  box = value;
}

// *************************************************************************
