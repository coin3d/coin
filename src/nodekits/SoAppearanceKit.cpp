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
  \class SoAppearanceKit SoAppearanceKit.h Inventor/nodekits/SoAppearanceKit.h
  \brief The SoAppearanceKit class ...
  \ingroup nodekits

  FIXME: write class doc
*/


#include <Inventor/nodekits/SoAppearanceKit.h>
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
  SO_KIT_INTERNAL_INIT_CLASS(SoAppearanceKit);
}
