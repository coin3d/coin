/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SoShapeSimplifyAction Inventor/include/SoShapeSimplifyAction.h
  \brief The SoShapeSimplifyAction class replaces complex primitives
  with simplified polygon representations.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoShapeSimplifyAction.h>
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
