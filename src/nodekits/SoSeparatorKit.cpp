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
  \class SoSeparatorKit SoSeparatorKit.h Inventor/nodekits/SoSeparatorKit.h
  \brief The SoSeparatorKit class ...
  \ingroup nodekits

  FIXME: write class doc
*/

#include <Inventor/nodekits/SoSeparatorKit.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoUnits.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTexture2Transform.h>
#include <Inventor/nodes/SoSeparator.h>


SO_KIT_SOURCE(SoSeparatorKit);


/*!
  Constructor.
*/
SoSeparatorKit::SoSeparatorKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoSeparatorKit);

  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, TRUE, this, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(pickStyle, SoPickStyle, TRUE, topSeparator, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(appearance, SoAppearanceKit, TRUE, topSeparator, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(units, SoUnits, TRUE, topSeparator, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(transform, SoTransform, TRUE, topSeparator, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(texture2Transform, SoTexture2Transform, TRUE, topSeparator, "", TRUE);
  SO_KIT_ADD_CATALOG_LIST_ENTRY(childList, SoSeparator, TRUE, topSeparator, "", SoShapeKit, TRUE);
  SO_KIT_ADD_LIST_ITEM_TYPE(childList, SoSeparatorKit);

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoSeparatorKit::~SoSeparatorKit()
{
}

/*!
  Does initialization common for all objects of the
  SoSeparatorKit class. This includes setting up the
  type system, among other things.
*/
void
SoSeparatorKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoSeparatorKit);
}


// FIXME: lots of code missing in this class. 19991120 mortene.
