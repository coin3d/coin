/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoSeparatorKit SoSeparatorKit.h Inventor/nodekits/SoSeparatorKit.h
  \brief The SoSeparatorKit class ...
  \ingroup nodekits

  FIXME: write class doc
*/

#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoSubKitP.h>
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

  SO_KIT_ADD_FIELD(renderCaching, (SoSeparatorKit::AUTO));
  SO_KIT_ADD_FIELD(boundingBoxCaching, (SoSeparatorKit::AUTO));
  SO_KIT_ADD_FIELD(renderCulling, (SoSeparatorKit::AUTO));
  SO_KIT_ADD_FIELD(pickCulling, (SoSeparatorKit::AUTO));

  SO_KIT_DEFINE_ENUM_VALUE(CacheEnabled, ON);
  SO_KIT_DEFINE_ENUM_VALUE(CacheEnabled, OFF);
  SO_KIT_DEFINE_ENUM_VALUE(CacheEnabled, AUTO);

  SO_KIT_SET_SF_ENUM_TYPE(renderCaching, CacheEnabled);
  SO_KIT_SET_SF_ENUM_TYPE(boundingBoxCaching, CacheEnabled);
  SO_KIT_SET_SF_ENUM_TYPE(renderCulling, CacheEnabled);
  SO_KIT_SET_SF_ENUM_TYPE(pickCulling, CacheEnabled);

  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, TRUE, this, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(pickStyle, SoPickStyle, TRUE, topSeparator, appearance, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(appearance, SoAppearanceKit, TRUE, topSeparator, units, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(units, SoUnits, TRUE, topSeparator, transform, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(transform, SoTransform, TRUE, topSeparator, texture2Transform, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(texture2Transform, SoTexture2Transform, TRUE, topSeparator, childList, TRUE);
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
