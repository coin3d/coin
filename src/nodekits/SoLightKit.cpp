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
  \class SoLightKit SoLightKit.h Inventor/nodekits/SoLightKit.h
  \brief The SoLightKit class provides a kit with a transform, a light and a shape or subgraph.
  \ingroup nodekits

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoLightKit
  -->"this"
        "callbackList"
  -->   "transformGroup"
  -->      "transform"
  -->      "light"
  -->      "iconSeparator"
  -->         "icon"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoLightKit
  PVT   "this",  SoLightKit  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "transformGroup",  SoTransformSeparator  --- 
        "transform",  SoTransform  --- 
        "light",  SoLight  --- , (default type = SoDirectionalLight)
  PVT   "iconSeparator",  SoSeparator  --- 
        "icon",  SoNode  --- , (default type = SoCube)
  \endverbatim

  \NODEKIT_POST_TABLE
*/

#include <Inventor/nodekits/SoLightKit.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>


SO_KIT_SOURCE(SoLightKit);


/*!
  Constructor.
*/
SoLightKit::SoLightKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoLightKit);

  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(transformGroup, SoTransformSeparator, TRUE, this, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(transform, SoTransform, TRUE, transformGroup, light, TRUE);
  SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(light, SoLight, SoDirectionalLight, FALSE, transformGroup, iconSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(iconSeparator, SoSeparator, TRUE, transformGroup, "", FALSE);
  SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(icon, SoNode, SoCube, TRUE, iconSeparator, "", TRUE);

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoLightKit::~SoLightKit()
{
}

// Documented in superclass.
void
SoLightKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoLightKit, SO_FROM_INVENTOR_1);
}
