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
  \class SoFontStyle SoFontStyle.h Inventor/nodes/SoFontStyle.h
  \brief The SoFontStyle class changes the appearance of fonts for text rendering nodes.
  \ingroup nodes

  Successive text rendering nodes will use fonts with the style
  settings of this node, if a font with the given settings can be
  found and loaded from the system.

  If the specified font style can not be found on the client system, a
  default fallback will be used. So the application programmer must
  consider SoFontStyle nodes as nodes giving \e hints about font
  settings, as you are \e not guaranteed to get exactly what you want.

  \since Inventor 2.1
  \sa SoFont, SoText2, SoText3
*/

// FIXME: this node will just be ignored yet due to capabilities
// missing in the SoText2 and SoText3 node implementations (they use
// hardcoded font data). 20000326 mortene.


#include <Inventor/nodes/SoFontStyle.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \enum SoFontStyle::Family
  Enumeration of the font family to use.
*/

/*!
  \enum SoFontStyle::Style
  Enumeration of font style characteristics.
*/

/*!
  \var SoSFEnum SoFontStyle::family
  Font family hint.
*/
/*!
  \var SoSFBitMask SoFontStyle::style
  Font style hint.
*/


// *************************************************************************

SO_NODE_SOURCE(SoFontStyle);

/*!
  Constructor.
*/
SoFontStyle::SoFontStyle(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoFontStyle);

  SO_NODE_ADD_FIELD(family, (SoFontStyle::SERIF));
  SO_NODE_ADD_FIELD(style, (SoFontStyle::NONE));

  SO_NODE_DEFINE_ENUM_VALUE(Family, SERIF);
  SO_NODE_DEFINE_ENUM_VALUE(Family, SANS);
  SO_NODE_DEFINE_ENUM_VALUE(Family, TYPEWRITER);
  SO_NODE_SET_SF_ENUM_TYPE(family, Family);

  SO_NODE_DEFINE_ENUM_VALUE(Style, NONE);
  SO_NODE_DEFINE_ENUM_VALUE(Style, BOLD);
  SO_NODE_DEFINE_ENUM_VALUE(Style, ITALIC);
  SO_NODE_SET_SF_ENUM_TYPE(style, Style);
}

/*!
  Destructor.
*/
SoFontStyle::~SoFontStyle()
{
}

// Doc from superclass.
void
SoFontStyle::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoFontStyle, SO_FROM_INVENTOR_2_1|SoNode::VRML1);
}

/*!
  Returns a system-specific text string to use for font loading, based
  on the style settings of this node.
*/
SbString
SoFontStyle::getFontName(void) const
{
  // FIXME: this is valid only under X11! 19990418 mortene.

  SbString fontname(this->name.getValue().getString());
  if (fontname == "defaultFont") {
    switch (this->family.getValue()) {
    case SoFontStyle::SERIF:
      fontname = "times";
      break;
    case SoFontStyle::SANS:
      fontname = "helvetica";
      break;
    case SoFontStyle::TYPEWRITER:
      fontname = "courier";
      break;
#if COIN_DEBUG
    default:
      SoDebugError::postWarning("SoFontStyle::getFontName",
                                "value of family field is invalid, "
                                "setting to SERIF");
      fontname = "times";
      break;
#endif // COIN_DEBUG
    }
  }

  // If this doesn't hold up, we need to include a few more cases in
  // the switch block.
  assert(SoFontStyle::NONE == 0);

  switch (this->style.getValue()) {
  case SoFontStyle::NONE:
    fontname += "-*-r-";
    break;
  case SoFontStyle::BOLD:
    fontname += "-bold-r-";
    break;
  case SoFontStyle::ITALIC:
    fontname += "-*-i-";
    break;
  case (SoFontStyle::BOLD | SoFontStyle::ITALIC):
    fontname += "-bold-i-";
    break;
#if COIN_DEBUG
  default:
    SoDebugError::postWarning("SoFontStyle::getFontName",
                              "value of style field is invalid");
    break;
#endif // COIN_DEUG
  }

  return fontname;
}

// Doc from superclass.
void
SoFontStyle::doAction(SoAction * action)
{
  SoFontNameElement::set(action->getState(), this, this->getFontName());
  SoFontSizeElement::set(action->getState(), this, this->size.getValue());
}

// Doc from superclass.
void
SoFontStyle::getBoundingBox(SoGetBoundingBoxAction * action)
{
  this->doAction(action);
}

// Doc from superclass.
void
SoFontStyle::GLRender(SoGLRenderAction * action)
{
  this->doAction(action);
}

// Doc from superclass.
void
SoFontStyle::callback(SoCallbackAction * action)
{
  SoFontStyle::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoFontStyle::pick(SoPickAction * action)
{
  SoFontStyle::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoFontStyle::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoFontStyle::doAction((SoAction *)action);
}
