/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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
