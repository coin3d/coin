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

#include "scxml/ScXMLFallbackInvoke.h"

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLStateMachine.h>
#include <Inventor/scxml/ScXMLEvent.h>

SCXML_OBJECT_SOURCE(ScXMLFallbackInvoke);

void
ScXMLFallbackInvoke::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(ScXMLFallbackInvoke, ScXMLInvoke, SCXML_DEFAULT_NS,
                          "sim.coin3d.coin", "Fallback");
}

ScXMLFallbackInvoke::ScXMLFallbackInvoke(void)
  : messaged(FALSE)
{
}

void
ScXMLFallbackInvoke::invoke(ScXMLStateMachine * statemachine)
{
#ifndef NDEBUG
  if (!this->messaged) {
    const char * eventtype = "<no event>";
    const char * targettypestring = "";
    const char * sourcestring = "";

    const ScXMLEvent * event = statemachine->getCurrentEvent();
    if (event) { eventtype = event->getIdentifier().getString(); }
    if (this->targettype) { targettypestring = this->targettype; }
    if (this->src) { sourcestring = this->src; }

    SoDebugError::post("ScXMLInvoke::invoke",
                       "Unbound <invoke> error: "
                       "targettype '%s', source '%s', event '%s'. "
                       "This message will not be repeated for this <invoke>.",
                       targettypestring, sourcestring, eventtype);
    this->messaged = TRUE;
  }
#endif // NDEBUG
}
