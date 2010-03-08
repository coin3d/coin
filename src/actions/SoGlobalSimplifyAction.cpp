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

// FIXME: this class has not yet been implemented in Coin. this is a
// placeholder / stub / skeleton code, not yet built (nor installed,
// obviously).  --mortene.

/*!
  \class SoGlobalSimplifyAction Inventor/include/SoGlobalSimplifyAction.h
  \brief The SoGlobalSimplifyAction class is for globally simplifying the
  geometry of a scene graph, globally.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoGlobalSimplifyAction.h>
#include <coindefs.h> // COIN_STUB()


SO_ACTION_SOURCE(SoGlobalSimplifyAction);

// Override from parent class.
void
SoGlobalSimplifyAction::initClass(void)
{
  SO_ACTION_INTERNAL_INIT_CLASS(SoGlobalSimplifyAction, SoSimplifyAction);
}


/*!
  A constructor.
*/

SoGlobalSimplifyAction::SoGlobalSimplifyAction(void)
{
  COIN_STUB();
}

/*!
  The destructor.
*/

SoGlobalSimplifyAction::~SoGlobalSimplifyAction(void)
{
  COIN_STUB();
}

// Documented in superclass.
void
SoGlobalSimplifyAction::beginTraversal(SoNode * /* node */)
{
  COIN_STUB();
}
