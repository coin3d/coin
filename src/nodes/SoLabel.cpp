/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
*/

#include <Inventor/nodes/SoLabel.h>
#include <Inventor/nodes/SoSubNodeP.h>


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
  SO_NODE_INTERNAL_INIT_CLASS(SoLabel, SO_FROM_INVENTOR_2_1);
}
