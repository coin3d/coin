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
  \class SoAppearanceKit SoAppearanceKit.h Inventor/nodekits/SoAppearanceKit.h
  \brief The SoAppearanceKit class is a node kit catalog that collects miscellaneous appearance node types.
  \ingroup nodekits

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoAppearanceKit
  -->"this"
        "callbackList"
  -->   "lightModel"
  -->   "environment"
  -->   "drawStyle"
  -->   "material"
  -->   "complexity"
  -->   "texture2"
  -->   "font"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoAppearanceKit
  PVT   "this",  SoAppearanceKit  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
        "lightModel",  SoLightModel  --- 
        "environment",  SoEnvironment  --- 
        "drawStyle",  SoDrawStyle  --- 
        "material",  SoMaterial  --- 
        "complexity",  SoComplexity  --- 
        "texture2",  SoTexture2  --- 
        "font",  SoFont  --- 
  \endverbatim

  \NODEKIT_POST_TABLE
*/


#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoFont.h>



SO_KIT_SOURCE(SoAppearanceKit);


/*!
  Constructor.
*/
SoAppearanceKit::SoAppearanceKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoAppearanceKit);

  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(lightModel, SoLightModel, TRUE, this, environment, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(environment, SoEnvironment, TRUE, this, drawStyle, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(drawStyle, SoDrawStyle, TRUE, this, material, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial, TRUE, this, complexity, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(complexity, SoComplexity, TRUE, this, texture2, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(texture2, SoTexture2, TRUE, this, font, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(font, SoFont, TRUE, this, "", TRUE);

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoAppearanceKit::~SoAppearanceKit()
{
}

// Documented in superclass.
void
SoAppearanceKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoAppearanceKit, SO_FROM_INVENTOR_1);
}
