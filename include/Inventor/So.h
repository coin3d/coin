#ifndef COIN_SO_H
#define COIN_SO_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SoPath.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>

#include <Inventor/misc/SoState.h>

#include <Inventor/SoLists.h>
#include <Inventor/actions/SoActions.h>
#include <Inventor/details/SoDetails.h>
#include <Inventor/elements/SoElements.h>
#include <Inventor/engines/SoEngines.h>
#include <Inventor/errors/SoErrors.h>
#include <Inventor/events/SoEvents.h>
#include <Inventor/fields/SoFields.h>
#include <Inventor/nodes/SoNodes.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <Inventor/sensors/SoSensors.h>

// We choose to not include classes that are "mostly internal":
// bundles, caches, elements, and projectors.
//
// Base classes are not included. They should for the most part be
// included by the other classes using them.
//
// Classes for interaction is not included either, as they can be seen
// as a later add-on to the core library, and it's not unlikely that
// we'll eventually make their inclusion a configure/build
// option. That goes for nodekits, draggers, and manips.
//
// The library can be built without VRML97 nodes, and without the
// thread abstraction classes, so they are also not part of this
// header file.

#endif // !COIN_SO_H
