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

#include <Inventor/scxml/ScXMLFinal.h>

#include <assert.h>

#include <Inventor/scxml/ScXML.h>

// *************************************************************************

/*!
  \class ScXMLFinal ScXMLFinal.h Inventor/scxml/ScXMLFinal.h
  \brief Implementation of the <final> SCXML element.

  \since Coin 3.0
  \ingroup scxml
*/

SCXML_OBJECT_SOURCE(ScXMLFinal);

void
ScXMLFinal::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLFinal, ScXMLStateBase, SCXML_DEFAULT_NS, "final");
}

ScXMLFinal::ScXMLFinal(void)
{
}

ScXMLFinal::~ScXMLFinal(void)
{
}
