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

/*!
  This method is called before a scene graph traversal.
*/

void
SoSimplifyAction::beginTraversal(SoNode * /* node */)
{
  COIN_STUB();
}
