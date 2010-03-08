/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoFontStyle SoFontStyle.h Inventor/nodes/SoFontStyle.h
  \brief The SoFontStyle class changes the appearance of fonts for text rendering nodes.
  \ingroup nodes

  Successive text rendering nodes will use fonts with the style
  settings of this node, if a font with the given settings can be
  found and loaded from the system.

  If the specified font style cannot be found on the client system, a
  default fallback will be used. So the application programmer must
  consider SoFontStyle nodes as nodes giving \e hints about font
  settings, as you are \e not guaranteed to get exactly what you want.

  Currently the SERIF family is mapped to "Times New Roman", SANS is
  mapped to "Arial" and TYPEWRITER is mapped to "Courier New".

  Please note that this node is inheriting \e SoFont. Previous font
  settings in a branch will therefore be overwritten with the default
  values of \e SoFont if an empty \e SoFontStyle node is inserted in
  the branch.

  \e NB! This node is provided for reasons of compatibility with Open
  Inventor 2.1 but will in most cases result in the default font being
  rendered. It is highly recommended to use the SoFont node instead.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    FontStyle {
        name "defaultFont"
        size 10
        family SERIF
        style ()
    }
  \endcode

  \since Inventor 2.1
  \sa SoFont, SoText2, SoText3, SoAsciiText, SoGlyph
*/

// *************************************************************************

#include <Inventor/nodes/SoFontStyle.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/errors/SoDebugError.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

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

  SbString fontname(this->name.getValue().getString());

#if COIN_DEBUG
  static SbBool messageflag = TRUE;
  if (messageflag && (fontname != "defaultFont")) {
    SoDebugError::postWarning("SoFontStyle::getFontName",
                              "Font name ('%s') is ignored when using "
                              "FontStyle nodes. Use the 'family' and "
                              "'style' fields instead.", 
                              fontname.getString());    
    messageflag = FALSE; // Only display this message once.
  }
#endif

  switch (this->family.getValue()) {
  case SoFontStyle::SERIF:
    fontname = "Times New Roman";
    break;
  case SoFontStyle::SANS:
    fontname = "Arial";
    break;
  case SoFontStyle::TYPEWRITER:
    fontname = "Courier New";
    break;
#if COIN_DEBUG
  default:
    SoDebugError::postWarning("SoFontStyle::getFontName",
                              "value of family field is invalid, setting to SERIF");
    fontname = "Times New Roman";
    break;
#endif // COIN_DEBUG
  }
  

  // If this doesn't hold up, we need to include a few more cases in
  // the switch block.
  assert(SoFontStyle::NONE == 0);

  switch (this->style.getValue()) {
  case SoFontStyle::NONE:
    break;
  case SoFontStyle::BOLD:
    fontname += ":Bold";
    break;
  case SoFontStyle::ITALIC:
    fontname += ":Italic";
    break;
  case (SoFontStyle::BOLD | SoFontStyle::ITALIC):
    fontname += ":Bold Italic";
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
  
  SoState * state = action->getState();
  uint32_t flags = SoOverrideElement::getFlags(state);
  
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)
  
  if (!name.isIgnored() && !TEST_OVERRIDE(FONT_NAME)) {
    SoFontNameElement::set(action->getState(), this, this->getFontName());
    if (this->isOverride()) {
      SoOverrideElement::setFontNameOverride(state, this, TRUE);
    }
  }
  if (!size.isIgnored() && !TEST_OVERRIDE(FONT_SIZE)) {
    SoFontSizeElement::set(action->getState(), this, this->size.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setFontSizeOverride(state, this, TRUE);
    }
  }

#undef TEST_OVERRIDE

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
