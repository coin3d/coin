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
  SO_NODE_INTERNAL_INIT_CLASS(SoInfo);
}
