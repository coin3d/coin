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

#include <Inventor/bundles/SoBundle.h>
#include <Inventor/actions/SoAction.h>

/*!
  \class SoBundle include/Inventor/bundles/SoBundle.h
  \brief The SoBundle class is the superclass for all bundle classes.
  \ingroup bundles
*/
// FIXME: document class properly (write general explanation of what
// bundles are used for. 20010828 mortene.


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
