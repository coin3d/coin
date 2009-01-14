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

#include "ScXML.cpp"
#include "ScXMLObject.cpp"
#include "ScXMLDocument.cpp"
#include "ScXMLState.cpp"
#include "ScXMLOnEntry.cpp"
#include "ScXMLOnExit.cpp"
#include "ScXMLInitial.cpp"
#include "ScXMLFinal.cpp"
#include "ScXMLAnchor.cpp"
#include "ScXMLHistory.cpp"
#include "ScXMLTransition.cpp"
#include "ScXMLEvent.cpp"
#include "ScXMLInvoke.cpp"
#include "ScXMLFallbackInvoke.cpp"
#include "ScXMLStateMachine.cpp"

#include "SoScXMLEvent.cpp"
#include "SoScXMLStateMachine.cpp"
#include "SoScXMLDebugInfoInvoke.cpp"
#include "SoScXMLScheduleRedrawInvoke.cpp"
#include "SoScXMLScheduleTimerEventInvoke.cpp"
