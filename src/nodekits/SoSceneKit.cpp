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
  \class SoSceneKit SoSceneKit.h Inventor/nodekits/SoSceneKit.h
  \brief The SoSceneKit class ...
  \ingroup nodekits

  FIXME: write class doc
*/

#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/nodekits/SoLightKit.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoSeparatorKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>


SO_KIT_SOURCE(SoSceneKit);


/*!
  Constructor.
*/
SoSceneKit::SoSceneKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoSceneKit);

  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, TRUE, this, "", FALSE);
  SO_KIT_ADD_CATALOG_LIST_ENTRY(cameraList, SoSwitch, TRUE, topSeparator, "", SoCameraKit, TRUE);
  SO_KIT_ADD_CATALOG_LIST_ENTRY(lightList, SoGroup, TRUE, topSeparator, "", SoLightKit, TRUE);
  SO_KIT_ADD_CATALOG_LIST_ENTRY(childList, SoGroup, TRUE, topSeparator, "", SoShapeKit, TRUE);
  SO_KIT_ADD_LIST_ITEM_TYPE(childList, SoSeparatorKit);

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoSceneKit::~SoSceneKit()
{
}

/*!
  Does initialization common for all objects of the
  SoSceneKit class. This includes setting up the
  type system, among other things.
*/
void
SoSceneKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoSceneKit);
}

/*!
  FIXME: write function documentation
*/
int
SoSceneKit::getCameraNumber(void)
{
  COIN_STUB();
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoSceneKit::setCameraNumber(int /*camNum*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSceneKit::affectsState(void) const
{
  COIN_STUB();
  return FALSE;
}
