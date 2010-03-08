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
  \class SoSFBox3d SoSFBox3d.h Inventor/fields/SoSFBox3d.h
  \brief The SoSFBox3d class is a container for an SbBox3d vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a box.

  \COIN_CLASS_EXTENSION
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoSFBox3d.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "fields/SoSubFieldP.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFBox3d, SbBox3d, const SbBox3d &);

// *************************************************************************

// Override from parent class.
void
SoSFBox3d::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFBox3d);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS


SbBool
SoSFBox3d::readValue(SoInput * in)
{
  double min[3];
  double max[3];
  if (!in->read(min[0]) ||
      !in->read(min[1]) ||
      !in->read(min[2]) ||
      !in->read(max[0]) ||
      !in->read(max[1]) ||
      !in->read(max[2])) {
    SoReadError::post(in, "Couldn't read SoSFBox3d");
    return FALSE;
  }
  this->setValue(min[0], min[1], min[2], max[0], max[1], max[2]);
  return TRUE;
}

void
SoSFBox3d::writeValue(SoOutput * out) const
{
  double min[3];
  double max[3];
  SbBox3d b = this->getValue();
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
SoSFBox3d::setValue(double xmin, double ymin, double zmin,
                    double xmax, double ymax, double zmax)
{
  this->setValue(SbBox3d(xmin, ymin, zmin, xmax, ymax, zmax));
}


/*!
  Set value of vector.
*/
void
SoSFBox3d::setValue(const SbVec3d & minvec, const SbVec3d & maxvec)
{
  this->setValue(SbBox3d(minvec, maxvec));
}


/*!
  Set value of vector.
*/
void
SoSFBox3d::getValue(SbBox3d & box) const
{
  box = value;
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFBox3d field;
  BOOST_CHECK_MESSAGE(SoSFBox3d::getClassTypeId() != SoType::badType(),
                      "SoSFBox3d class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
