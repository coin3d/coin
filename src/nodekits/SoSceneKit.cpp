/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoSceneKit SoSceneKit.h Inventor/nodekits/SoSceneKit.h
  \brief The SoSceneKit class collects node kits needed to set up a scene: camera, light and shapes.
  \ingroup nodekits

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoSceneKit
  -->"this"
        "callbackList"
  -->   "topSeparator"
  -->      "cameraList"
  -->      "lightList"
  -->      "childList"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoSceneKit
  PVT   "this",  SoSceneKit  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "topSeparator",  SoSeparator  --- 
        "cameraList",  SoNodeKitListPart [ SoCameraKit ] 
        "lightList",  SoNodeKitListPart [ SoLightKit ] 
        "childList",  SoNodeKitListPart [ SoShapeKit, SoSeparatorKit ] 
  \endverbatim

  \NODEKIT_POST_TABLE
*/

#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/nodekits/SoLightKit.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_KIT_SOURCE(SoSceneKit);


/*!
  Constructor.
*/
SoSceneKit::SoSceneKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoSceneKit);

  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, TRUE, this, "", FALSE);
  SO_KIT_ADD_CATALOG_LIST_ENTRY(cameraList, SoSwitch, TRUE, topSeparator, lightList, SoCameraKit, TRUE);
  SO_KIT_ADD_CATALOG_LIST_ENTRY(lightList, SoGroup, TRUE, topSeparator, childList, SoLightKit, TRUE);
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

// doc from superclass
void
SoSceneKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoSceneKit, SO_FROM_INVENTOR_1);
}

/*!
  Returns the index of the current active camera in cameraList.
*/
int
SoSceneKit::getCameraNumber(void)
{
  SoSwitch *sw = (SoSwitch*)this->getContainerNode(SbName("cameraList"));
  return sw->whichChild.getValue();
}

/*!
  Sets the current active camera in cameraList.
*/
void
SoSceneKit::setCameraNumber(int camnum)
{
  SoSwitch *sw = (SoSwitch*)this->getContainerNode(SbName("cameraList"));
#if COIN_DEBUG && 1 // debug
  if (camnum >= sw->getNumChildren()) {
    SoDebugError::postInfo("SoSceneKit::setCameraNumber",
                           "camera number %d is too large", camnum);
    return;
  }
#endif // debug
  sw->whichChild = camnum;
}

// Documented in superclass.
SbBool
SoSceneKit::affectsState(void) const
{
  return TRUE;
}
