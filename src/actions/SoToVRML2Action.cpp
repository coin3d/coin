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
  \class SoToVRML2Action Inventor/include/SoToVRML2Action.h
  \brief The SoToVRML2Action class builds a new scene graph, using only
  VRML 2.0 nodes.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoToVRML2Action.h>
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
