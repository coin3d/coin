/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/nodes/SoViewerNavigationMode.h>

#ifndef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/nodes/SoSubNode.h>

// *************************************************************************

/*!
  \class SoViewerNavigationMode Inventor/nodes/SoViewerNavigationMode.h
  \brief Node for setting the default viewer navigation mode for a scene graph.

  This node provides a way to hint to the SoSceneManager (or SoEventManager)
  which navigation mode you want to use with a scene graph.

  The node must be placed before the first camera in the scene graph,
  or it will not be found by the system that searches for this node.

  Currently, the mode field of the node is not monitored for dynamic
  changes, so updating it will not change the viewer mode.  Doing this is
  a possibility for future enhancement.

  \ingroup nodes
  \ingroup navigation
*/

/*!
  \var SoSFString SoViewerNavigationMode::mode

  The mode field is used for setting the name of the desired mode.
  If the mode is not found in the system, no mode will be used.

  The builtin modes are currently limited to "examiner", "panner",
  "zoomer", and "rotater".  In addition, application developers can
  extend the list with their own custom modes.
*/

#define PRIVATE(obj) ((obj)->pimpl)

SO_NODE_SOURCE(SoViewerNavigationMode);

/*!
  Static class initializer.  It is called from SoNavigation::init, so
  there should be no need to call this directly.
*/

void
SoViewerNavigationMode::initClass(void)
{
  SO_NODE_INIT_CLASS(SoViewerNavigationMode, SoNode, SoNode);
}

/*!
  Constructor.
*/

SoViewerNavigationMode::SoViewerNavigationMode(void)
{
  PRIVATE(this) = NULL; // reserved for future
  SO_NODE_CONSTRUCTOR(SoViewerNavigationMode);
  SO_NODE_ADD_FIELD(mode, (""));
}

/*!
  Destructor.
*/

SoViewerNavigationMode::~SoViewerNavigationMode(void)
{
}

#undef PRIVATE
