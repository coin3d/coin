/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoSFBool SoSFBool.h Inventor/fields/SoSFBool.h
  \brief The SoSFBool class is a container for an SbBool value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a boolean on/off or TRUE/FALSE value, like for
  instance as the "on" field of the SoPointLight, SoSpotLight and
  SoDirectionalLight node classes.


  \sa SoMFBool
*/

// *************************************************************************

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFBool, SbBool, SbBool);

// *************************************************************************

// Override from parent.
void
SoSFBool::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFBool);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFBool::readValue(SoInput * in)
{
  SbBool val;
  if (!sosfbool_read_value(in, val)) return FALSE;
  this->value = val;
  return TRUE;
}

void
SoSFBool::writeValue(SoOutput * out) const
{
  sosfbool_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************
