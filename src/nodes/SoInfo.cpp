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

/*!
  \class SoInfo SoInfo.h Inventor/nodes/SoInfo.h
  \brief The SoInfo class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoInfo.h>



/*!
  \var SoSFString SoInfo::string
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoInfo);

/*!
  Constructor.
*/
SoInfo::SoInfo()
{
  SO_NODE_CONSTRUCTOR(SoInfo);

  SO_NODE_ADD_FIELD(string, ("<Undefined info>"));
}

/*!
  Destructor.
*/
SoInfo::~SoInfo()
{
}

/*!
  Does initialization common for all objects of the
  SoInfo class. This includes setting up the
  type system, among other things.
*/
void
SoInfo::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoInfo);
}
