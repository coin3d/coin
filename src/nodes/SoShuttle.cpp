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
  \class SoShuttle SoShuttle.h Inventor/nodes/SoShuttle.h
  \brief The SoShuttle class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoShuttle.h>



/*!
  \var SoSFVec3f SoShuttle::translation0
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoShuttle::translation1
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoShuttle::speed
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoShuttle::on
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoShuttle);

/*!
  Constructor.
*/
SoShuttle::SoShuttle()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShuttle);

  SO_NODE_ADD_FIELD(translation0, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_NODE_ADD_FIELD(translation1, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_NODE_ADD_FIELD(speed, (1.0f));
  SO_NODE_ADD_FIELD(on, (TRUE));
}

/*!
  Destructor.
*/
SoShuttle::~SoShuttle()
{
}

/*!
  Does initialization common for all objects of the
  SoShuttle class. This includes setting up the
  type system, among other things.
*/
void
SoShuttle::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShuttle);
}
