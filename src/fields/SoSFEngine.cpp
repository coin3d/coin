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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoSFEngine SoSFEngine.h Inventor/fields/SoSFEngine.h
  \brief The SoSFEngine class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFEngine.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/engines/SoEngine.h>
#include <coindefs.h> // COIN_STUB()
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


// FIXME: can we use SO_SFIELD_SOURCE() instead? Depends on whether or
// not we should do reference counting, I think. 19991227 mortene.
SO_SFIELD_REQUIRED_SOURCE(SoSFEngine);


// Doc from superclass
void
SoSFEngine::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFEngine);
}

// (Declarations hidden in SO_[S|M]FIELD_HEADER macro in header file,
// so don't use Doxygen commenting.)
#ifndef DOXYGEN_SKIP_THIS

/* Constructor. */
SoSFEngine::SoSFEngine(void)
{
  this->value = NULL;
}

/* Destructor. */
SoSFEngine::~SoSFEngine(void)
{
  this->enableNotify(FALSE);
  this->setValue(NULL);
}

/* FIXME: write function documentation */
SbBool
SoSFEngine::readValue(SoInput * /* in */)
{
  COIN_STUB();
  return FALSE;
}

/* FIXME: write function documentation */
void
SoSFEngine::writeValue(SoOutput * /* out */) const
{
  COIN_STUB();
}

/* FIXME: write function documentation */
void
SoSFEngine::setValue(SoEngine * value)
{
  // FIXME: should we really ref/unref here? 19990630 mortene.

  if (this->value) this->value->unref();
  this->value = value;
  if (this->value) this->value->ref();
  this->valueChanged();
}

/* FIXME: write function documentation */
SbBool
SoSFEngine::operator == (const SoSFEngine & field) const
{
  return (this->getValue() == field.getValue());
}

#endif // DOXYGEN_SKIP_THIS

/*!
  FIXME: write function documentation
*/
void
SoSFEngine::fixCopy(SbBool /* copyConnections */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFEngine::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

// Overridden from parent to propagate write reference counting to
// value member.
void
SoSFEngine::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);
  COIN_STUB();
}
