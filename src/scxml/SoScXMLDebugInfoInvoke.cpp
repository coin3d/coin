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

#include "scxml/SoScXMLDebugInfoInvoke.h"

#include <assert.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLStateMachine.h>
#include <Inventor/scxml/ScXMLEvent.h>

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLDebugInfoInvoke);

void
SoScXMLDebugInfoInvoke::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLDebugInfoInvoke, ScXMLInvoke, SCXML_COIN_NS,
                          "sim.coin3d.coin", "DebugInfo");
}

SoScXMLDebugInfoInvoke::SoScXMLDebugInfoInvoke(void)
{
}

void
SoScXMLDebugInfoInvoke::invoke(ScXMLStateMachine * statemachinearg)
{
  assert(statemachinearg);
  if (this->srcexpr) {
    SoDebugError::postInfo("ScXML::DebugInfo", "%s", this->srcexpr);
  } else {
    const ScXMLEvent * curevent = statemachinearg->getCurrentEvent();
    if (curevent) {
      SoDebugError::postInfo("ScXML::DebugInfo", "<no info> <%s>",
                             curevent->getIdentifier().getString());
    } else {
      SoDebugError::postInfo("ScXML::DebugInfo", "<no info>");
    }
  }
}
