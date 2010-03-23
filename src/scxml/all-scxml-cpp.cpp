/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

// Utils
#include "SbStringConvert.cpp"

// Infrastructure
#include "ScXML.cpp"
#include "ScXMLObject.cpp"
#include "ScXMLEvent.cpp"
#include "ScXMLEventTarget.cpp"
#include "ScXMLStateMachine.cpp"
#include "ScXMLDocument.cpp"

// Core Module
#include "ScXMLElt.cpp"
#include "ScXMLScxmlElt.cpp"
#include "ScXMLInitialElt.cpp"
#include "ScXMLAbstractStateElt.cpp"
#include "ScXMLStateElt.cpp"
#include "ScXMLParallelElt.cpp"
#include "ScXMLFinalElt.cpp"
#include "ScXMLOnEntryElt.cpp"
#include "ScXMLOnExitElt.cpp"
#include "ScXMLTransitionElt.cpp"
#include "ScXMLHistoryElt.cpp"

// Executable Content
#include "ScXMLExecutableElt.cpp"
#include "ScXMLEventElt.cpp"
#include "ScXMLIfElt.cpp"
#include "ScXMLElseIfElt.cpp"
#include "ScXMLElseElt.cpp"
#include "ScXMLLogElt.cpp"

// External Communication Module
#include "ScXMLSendElt.cpp"
#include "ScXMLInvokeElt.cpp"
#include "ScXMLParamElt.cpp"
#include "ScXMLFinalizeElt.cpp"
#include "ScXMLContentElt.cpp"

// Data Module
#include "ScXMLDataElt.cpp"
#include "ScXMLDataModelElt.cpp"
#include "ScXMLAssignElt.cpp"
#include "ScXMLValidateElt.cpp"

// Script Module
#include "ScXMLScriptElt.cpp"

// Anchor Module
#include "ScXMLAnchorElt.cpp"

// Expression Evaluation
#include "ScXMLEvaluator.cpp"

#include "ScXMLMinimumEvaluator.cpp"
#include "eval-minimum-tab.cpp"
#include "eval-minimum.cpp"
#include "ScXMLXPathEvaluator.cpp"
#include "ScXMLECMAScriptEvaluator.cpp"

