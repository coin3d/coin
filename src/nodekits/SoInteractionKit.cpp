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
  \class SoInteractionKit SoInteractionKit.h Inventor/nodekits/SoInteractionKit.h
  \brief The SoInteractionKit class ...
  \ingroup nodekits

  FIXME: write class doc
*/

#include <Inventor/nodekits/SoInteractionKit.h>

SO_KIT_SOURCE(SoInteractionKit);

/*!
  Constructor.
*/
SoInteractionKit::SoInteractionKit(void)
{
  SO_KIT_CONSTRUCTOR(SoInteractionKit);

  // FIXME: need to define catalog here. 19991107 mortene.

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoInteractionKit::~SoInteractionKit()
{
}

/*!
  Does initialization common for all objects of the
  SoInteractionKit class. This includes setting up the
  type system, among other things.
*/
void
SoInteractionKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoInteractionKit);
}
