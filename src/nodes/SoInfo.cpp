/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoInfo SoInfo.h Inventor/nodes/SoInfo.h
  \brief The SoInfo class is a node for holding text information.
  \ingroup nodes

  This is simply a placeholder for textual information about a
  scene. Typically used for specifying author and copyright
  information in Inventor format files, but can also be used for other
  purposes, of course.

  SoInfo nodes does not influence traversal in any way for any of the
  builtin actions.
*/

#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoSubNodeP.h>

/*!
  \var SoSFString SoInfo::string
  Text information.
*/

// *************************************************************************

SO_NODE_SOURCE(SoInfo);

/*!
  Constructor.
*/
SoInfo::SoInfo(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoInfo);

  SO_NODE_ADD_FIELD(string, ("<Undefined info>"));
}

/*!
  Destructor.
*/
SoInfo::~SoInfo()
{
}

// Doc from superclass.
void
SoInfo::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoInfo, SO_FROM_INVENTOR_1|SoNode::VRML1);
}
