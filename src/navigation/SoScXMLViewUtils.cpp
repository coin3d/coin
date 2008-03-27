/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
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

#include "navigation/SoScXMLViewUtils.h"

#include <Inventor/SbViewportRegion.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/scxml/SoScXMLStateMachine.h>
#include <Inventor/scxml/ScXML.h>

SCXML_OBJECT_SOURCE(SoScXMLViewAll);

void
SoScXMLViewAll::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLViewAll, SoScXMLNavigationInvoke,
                          SCXML_COIN_NS, "sim.coin3d.coin", "ViewAll");
}

void
SoScXMLViewAll::invoke(const ScXMLStateMachine * statemachinearg) const
{
  assert(statemachinearg);
  if (!statemachinearg->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    return;
  }

  const SoScXMLStateMachine * statemachine =
    static_cast<const SoScXMLStateMachine *>(statemachinearg);

  const SbViewportRegion & viewport = statemachine->getViewportRegion();
  SoCamera * camera = statemachine->getActiveCamera();
  if (!camera) return;
  SoNode * scenegraph = statemachine->getSceneGraphRoot();
  if (!scenegraph) return;
  camera->viewAll(scenegraph, viewport);
}
