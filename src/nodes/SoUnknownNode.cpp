/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

// This is an internal class for holding user extension nodes from .iv
// files. The header file for this node is not exported at ``make
// install'', and the class is not documented for application
// programmers.

#include <Inventor/nodes/SoUnknownNode.h>


// FIXME: needs dynamic SoFieldData instances to store the fields?
// 20000102 mortene.
SO_NODE_SOURCE(SoUnknownNode);


SoUnknownNode::SoUnknownNode(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoUnknownNode);
}

SoUnknownNode::~SoUnknownNode()
{
  // FIXME: release fields allocated by the import code? 20000102 mortene.
}

void
SoUnknownNode::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoUnknownNode);
}

SbBool
SoUnknownNode::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: check "flags" argument to see if we should behave like a
  // group node? 20000102 mortene.
  return inherited::readInstance(in, flags);
}

// The name used for the nodes of this type in scene file.
void
SoUnknownNode::setNodeClassName(const SbName & name)
{
  this->classname = name;
}

// Overridden from SoBase.
const char *
SoUnknownNode::getFileFormatName(void) const
{
  return this->classname.getString();
}
