/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
