/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoSFPath SoSFPath.h Inventor/fields/SoSFPath.h
  \brief The SoSFPath class is a container for an SoPath object.
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFPath.h>
#include <Inventor/fields/SoSubFieldP.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/errors/SoReadError.h>
#include <coindefs.h> // COIN_STUB()
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFPath, SoPath *, SoPath *);


// Override from parent class.
void
SoSFPath::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFPath);
}

void
SoSFPath::notify(SoNotList * l)
{
  COIN_STUB();
}

void
SoSFPath::fixCopy(SbBool copyconnections)
{
  COIN_STUB();
}

SbBool
SoSFPath::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFPath::readValue(SoInput * in)
{
  SoBase * baseptr;
  if (!SoBase::read(in, baseptr, SoPath::getClassTypeId())) return FALSE;

  if (in->eof()) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }

  this->setValue((SoPath *)baseptr);
  return TRUE;
}

void
SoSFPath::writeValue(SoOutput * out) const
{
  SoPath * path = this->getValue();
  if (path) {
    SoWriteAction wa(out);
    path->write(&wa);
  }
  else {
    // This actually works for both ASCII and binary formats.
    out->write("NULL");
  }
}

#endif // DOXYGEN_SKIP_THIS

// Overridden from parent to propagate write reference counting to
// value member.
void
SoSFPath::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);

  SoPath * p = this->getValue();
  // Set the "from field" flag as FALSE, is that flag is meant to be
  // used for references through field-to-field connections.
  if (p) {
    SoWriteAction wa(out);
    p->write(&wa);
  }
}
