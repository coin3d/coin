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
  \class SoAppearanceKit SoAppearanceKit.h Inventor/nodekits/SoAppearanceKit.h
  \brief The SoAppearanceKit class ...
  \ingroup nodekits

  FIXME: write class doc
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
  SO_KIT_ADD_CATALOG_ENTRY(environment, SoEnvironment, TRUE, this, drawStyle, TRUE);SO_KIT_ADD_CATALOG_ENTRY(drawStyle, SoDrawStyle, TRUE, this, material, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial, TRUE, this, complexity, TRUE);SO_KIT_ADD_CATALOG_ENTRY(complexity, SoComplexity, TRUE, this, texture2, TRUE);
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

/*!
  Does initialization common for all objects of the
  SoAppearanceKit class. This includes setting up the
  type system, among other things.
*/
void
SoAppearanceKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoAppearanceKit, SO_FROM_INVENTOR_2_1);
}
