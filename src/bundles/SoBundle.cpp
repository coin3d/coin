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

#include <Inventor/bundles/SoBundle.h>
#include <Inventor/actions/SoAction.h>

/*!
  \class SoBundle include/Inventor/bundles/SoBundle.h
  \brief The SoBundle class is the superclass for all bundle classes.

  bla, bla, bla, FIXME: doc
*/

/*!
  Constructor with action being the action applied to the node.
*/
SoBundle::SoBundle(SoAction *action)
{
  this->state = action->getState();
}

/*!
  Destructor
*/
SoBundle::~SoBundle()
{
}

/*!
  \var SoBundle::state
  \brief The state of the current action
*/
