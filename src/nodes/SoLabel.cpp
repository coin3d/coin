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

/*!
  \class SoLabel SoLabel.h Inventor/nodes/SoLabel.h
  \brief The SoLabel class is a node for holding label information in a scene graph.
  \ingroup nodes

  Use this node as a convenient way of labeling nodes or subgraphs
  within a scene graph.

  SoLabel nodes does not influence traversal in any way for any of the
  builtin actions.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    Label {
        label "<Undefined label>"
    }
  \endcode
*/

// *************************************************************************

#include <Inventor/nodes/SoLabel.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

/*!
  \var SoSFName SoLabel::label
  Label text.
*/

// *************************************************************************

SO_NODE_SOURCE(SoLabel);

/*!
  Constructor.
*/
SoLabel::SoLabel(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLabel);

  SO_NODE_ADD_FIELD(label, ("<Undefined label>"));
}

/*!
  Destructor.
*/
SoLabel::~SoLabel()
{
}

// Doc from superclass.
void
SoLabel::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLabel, SO_FROM_INVENTOR_1);
}
