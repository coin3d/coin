/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
