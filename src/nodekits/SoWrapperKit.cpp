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
  \class SoWrapperKit SoWrapperKit.h Inventor/nodekits/SoWrapperKit.h
  \brief The SoWrapperKit class ...
  \ingroup nodekits

  FIXME: write class doc
*/

#include <Inventor/nodekits/SoWrapperKit.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>


SO_KIT_SOURCE(SoWrapperKit);


/*!
  Constructor.
*/
SoWrapperKit::SoWrapperKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoWrapperKit);

  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(localTransform, SoTransform, TRUE, topSeparator, contents, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(contents, SoSeparator, TRUE, topSeparator, "", TRUE);

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoWrapperKit::~SoWrapperKit()
{
}

/*!
  Does initialization common for all objects of the
  SoWrapperKit class. This includes setting up the
  type system, among other things.
*/
void
SoWrapperKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoWrapperKit);
}
