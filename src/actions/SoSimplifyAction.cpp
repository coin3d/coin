/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoSimplifyAction Inventor/include/SoSimplifyAction.h
  \brief The SoSimplifyAction class is the base class for the simplify
  action classes.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoSimplifyAction.h>
#include <coindefs.h> // COIN_STUB()


SO_ACTION_SOURCE(SoSimplifyAction);

// Overridden from parent class.
void
SoSimplifyAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoSimplifyAction, SoAction);
}


/*!
  A constructor.
*/

SoSimplifyAction::SoSimplifyAction(void)
{
  COIN_STUB();
}

/*!
  The destructor.
*/

SoSimplifyAction::~SoSimplifyAction(void)
{
}

// Documented in superclass.
void
SoSimplifyAction::beginTraversal(SoNode * /* node */)
{
  COIN_STUB();
}
