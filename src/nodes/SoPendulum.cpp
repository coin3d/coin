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
  \class SoPendulum SoPendulum.h Inventor/nodes/SoPendulum.h
  \brief The SoPendulum class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPendulum.h>


#include <Inventor/SbVec3f.h>

/*!
  \var SoSFRotation SoPendulum::rotation0
  FIXME: write documentation for field
*/
/*!
  \var SoSFRotation SoPendulum::rotation1
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoPendulum::speed
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoPendulum::on
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoPendulum);

/*!
  Constructor.
*/
SoPendulum::SoPendulum()
{
  SO_NODE_CONSTRUCTOR(SoPendulum);

  SO_NODE_ADD_FIELD(rotation0, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(rotation1, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(speed, (1.0f));
  SO_NODE_ADD_FIELD(on, (TRUE));
}

/*!
  Destructor.
*/
SoPendulum::~SoPendulum()
{
}

/*!
  Does initialization common for all objects of the
  SoPendulum class. This includes setting up the
  type system, among other things.
*/
void
SoPendulum::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPendulum);
}
