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
  \class SoSFPath SoSFPath.h Inventor/fields/SoSFPath.h
  \brief The SoSFPath class is a container for an SoPath object.
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFPath.h>
#include <SoSubFieldPrivate.h>

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
