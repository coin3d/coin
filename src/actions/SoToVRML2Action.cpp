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
  \class SoToVRML2Action Inventor/include/SoToVRML2Action.h
  \brief The SoToVRML2Action class builds a new scene graph, using only
  VRML 2.0 nodes.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoToVRML2Action.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <coindefs.h> // COIN_STUB()


SO_ACTION_SOURCE(SoToVRML2Action);

// Overridden from parent class.
void
SoToVRML2Action::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoToVRML2Action, SoToVRMLAction);
}


/*!
  A constructor.
*/

SoToVRML2Action::SoToVRML2Action(void)
{
  COIN_STUB();
}

/*!
  The destructor.
*/

SoToVRML2Action::~SoToVRML2Action(void)
{
  // FIXME: implement.
}

/*!
  This method is called before the scene graph traversal is begun.
*/

void
SoToVRML2Action::beginTraversal(SoNode * /* node */)
{
  COIN_STUB();
}
