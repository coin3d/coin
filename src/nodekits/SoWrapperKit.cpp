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
