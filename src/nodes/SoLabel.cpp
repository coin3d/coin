/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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
  SO_NODE_INTERNAL_INIT_CLASS(SoLabel);
}
