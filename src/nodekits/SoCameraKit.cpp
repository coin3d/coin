/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoCameraKit SoCameraKit.h Inventor/nodekits/SoCameraKit.h
  \brief The SoCameraKit class is a node kit with a transform and a camera.
  \ingroup nodekits

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoCameraKit
  -->"this"
        "callbackList"
  -->   "transformGroup"
  -->      "transform"
  -->      "camera"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoCameraKit
  PVT   "this",  SoCameraKit  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "transformGroup",  SoTransformSeparator  --- 
        "transform",  SoTransform  --- 
        "camera",  SoCamera  --- , (default type = SoPerspectiveCamera)
  \endverbatim

  \NODEKIT_POST_TABLE
*/

#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>


SO_KIT_SOURCE(SoCameraKit);


/*!
  Constructor.
*/
SoCameraKit::SoCameraKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoCameraKit);

  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(transformGroup, SoTransformSeparator, TRUE, this, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(transform, SoTransform, TRUE, transformGroup, camera, TRUE);
  SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(camera, SoCamera, SoPerspectiveCamera, FALSE, transformGroup, "", TRUE);

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoCameraKit::~SoCameraKit()
{
}

// Documented in superclass.
void
SoCameraKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoCameraKit, SO_FROM_INVENTOR_1);
}
