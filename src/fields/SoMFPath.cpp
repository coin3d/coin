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
  \class SoMFPath SoMFPath.h Inventor/fields/SoMFPath.h
  \brief The SoMFPath class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFPath.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <coindefs.h> // COIN_STUB()
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFPath, SoPath *, SoPath *);


// Doc from superclass
void
SoMFPath::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFPath);
}

#ifndef DOXYGEN_SKIP_THIS

/* FIXME: write function documentation */
SbBool
SoMFPath::read1Value(SoInput * /* in */, int /* idx */)
{
  COIN_STUB();
  return FALSE;
}

/* FIXME: write function documentation */
void
SoMFPath::write1Value(SoOutput * /* out */, int /* idx */) const
{
  COIN_STUB();
}

#endif // DOXYGEN_SKIP_THIS

/*!
  FIXME: write function documentation
*/
void
SoMFPath::notify(SoNotList * /* list */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoMFPath::fixCopy(SbBool /* copyConnections */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFPath::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

// Overridden from parent to propagate write reference counting to
// value members.
void
SoMFPath::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);
  COIN_STUB();
}
