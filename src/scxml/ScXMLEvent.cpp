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

#include <Inventor/scxml/ScXMLEvent.h>

#include <assert.h>
#include <string.h>

#include <Inventor/scxml/ScXML.h>

// *************************************************************************

/*!
  \class ScXMLEvent ScXMLEvent.h Inventor/scxml/ScXMLEvent.h
  \brief Base class for events sent to SCXML state machines.

  \since Coin 3.0
  \ingroup scxml
*/

SCXML_OBJECT_SOURCE(ScXMLEvent);

void
ScXMLEvent::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLEvent, ScXMLObject, SCXML_DEFAULT_NS, "event");
}

// *************************************************************************

ScXMLEvent::ScXMLEvent(void)
  : identifier(SbName::empty())
{
}

ScXMLEvent::~ScXMLEvent(void)
{
}

// *************************************************************************

/*!
  This method is for setting a string that will identifies this particular
  event, having this particular state, and can be used for event matching
  in the SCXML descriptions.

  The string should, according to spec., be a set of tokens consisting
  of alphanumeric characters, separated with periods (.). This limitation
  is not enforced by this implementation.
*/
void
ScXMLEvent::setIdentifier(const SbName & identifierarg)
{
  this->identifier = identifierarg;
}

// *************************************************************************
