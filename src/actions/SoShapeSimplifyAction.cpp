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
  \class SoShapeSimplifyAction Inventor/include/SoShapeSimplifyAction.h
  \brief The SoShapeSimplifyAction class replaces complex primitives
  with simplified polygon representations.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoShapeSimplifyAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <coindefs.h> // COIN_STUB()


SO_ACTION_SOURCE(SoShapeSimplifyAction);

// Overridden from parent class.
void
SoShapeSimplifyAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoShapeSimplifyAction, SoSimplifyAction);
}


/*!
  A constructor.
*/

SoShapeSimplifyAction::SoShapeSimplifyAction(void)
{
  COIN_STUB();
}

/*!
  The destructor.
*/

SoShapeSimplifyAction::~SoShapeSimplifyAction(void)
{
  COIN_STUB();
}

/*!
  This method is called at the beginning of the traversal.
*/

void
SoShapeSimplifyAction::beginTraversal(SoNode * /* node */)
{
  COIN_STUB();
}
