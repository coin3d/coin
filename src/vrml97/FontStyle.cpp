/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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
  \class SoVRMLFontStyle SoVRMLFontStyle.h Inventor/VRMLnodes/SoVRMLFontStyle.h
  \brief The SoVRMLFontStyle class is used to define the current font.
  \ingroup VRMLnodes

  FIXME: implement and document

*/

/*!
  SoSFFloat SoVRMLFontStyle::size
  Size of font.
*/

/*!
  SoMFString SoVRMLFontStyle::family
  Font family. All browsers must support "SANS", "SERIF" and "TYPEWRITER". Default value is "SERIF".
*/

/*!
  SoMFString SoVRMLFontStyle::style
  Font style. Can be one of "PLAIN", "BOLD", "ITALIC" or "BOLDITALIC". Default value is "PLAIN".
*/

/*!
  SoSFBool SoVRMLFontStyle::horizontal
  TRUE if strings should be rendered horizontally. Default value is TRUE.
*/

/*!
  SoSFBool SoVRMLFontStyle::leftToRight
  TRUE if strings should be rendered left to right. Default value is TRUE.
*/

/*!
  SoSFBool SoVRMLFontStyle::topToBottom
  True if strings should be rendered top to bottom. Default value is TRUE.
*/

/*!
  SoSFString SoVRMLFontStyle::language
  Text language. Empty by default.
*/

/*!
  SoMFString SoVRMLFontStyle::justify
  Text justification. Can be "BEGIN", "FIRST" "MIDDLE" or "END". Default value is "BEGIN".
*/

/*!
  SoSFFloat SoVRMLFontStyle::spacing
  Spacing constant. Default value is 1.0.
*/


#include <Inventor/VRMLnodes/SoVRMLFontStyle.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

SO_NODE_SOURCE(SoVRMLFontStyle);

// Doc in parent
void
SoVRMLFontStyle::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLFontStyle, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLFontStyle::SoVRMLFontStyle(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLFontStyle);

  SO_VRMLNODE_ADD_FIELD(size, (1.0f));
  SO_VRMLNODE_ADD_FIELD(family, ("SERIF"));
  SO_VRMLNODE_ADD_FIELD(style, ("PLAIN"));
  SO_VRMLNODE_ADD_FIELD(horizontal, (TRUE));
  SO_VRMLNODE_ADD_FIELD(leftToRight, (TRUE));
  SO_VRMLNODE_ADD_FIELD(topToBottom, (TRUE));
  SO_VRMLNODE_ADD_FIELD(language, (""));
  SO_VRMLNODE_ADD_FIELD(justify, ("BEGIN"));
  SO_VRMLNODE_ADD_FIELD(spacing, (1.0f));
}

/*!
  Destructor.
*/
SoVRMLFontStyle::~SoVRMLFontStyle()
{
}

SbString
SoVRMLFontStyle::getFontName(void)
{
  return SbString("");
}

// Doc in parent
void
SoVRMLFontStyle::doAction(SoAction * action)
{
}

// Doc in parent
void
SoVRMLFontStyle::callback(SoCallbackAction * action)
{
}

// Doc in parent
void
SoVRMLFontStyle::GLRender(SoGLRenderAction * action)
{
}

// Doc in parent
void
SoVRMLFontStyle::getBoundingBox(SoGetBoundingBoxAction * action)
{
}

// Doc in parent

void
SoVRMLFontStyle::pick(SoRayPickAction * action)
{
}

// Doc in parent
void
SoVRMLFontStyle::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
}
