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
  \class SoFont SoFont.h Inventor/nodes/SoFont.h
  \brief The SoFont class is an appearance node for setting fonts.
  \ingroup nodes

  Successive text rendering nodes (like SoText2, SoText3, SoAsciiText,
  etc) will use the font specified from an SoFont node when
  visualizing text.

  \sa SoFontStyle, SoText2, SoText3
*/

#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoOverrideElement.h>

/*!
  \var SoSFName SoFont::name

  Name of font.

  Which fontnames are available is rather systemdependent, not only on
  whether or not you are running on a UNIX/Linux system, Microsoft
  Windows or whatever, but also on which fonts and font \e types (like
  TrueType) are installed on a particular user's system.

  All font rendering nodes have a built-in fallback font to use,
  though, so even though Coin can not find a font on the system of the
  specified type, the text should be rendered somehow.

  In summation, consider this node type and this particular field as a
  \e hint to the font rendering engines of Coin, and do \e not base
  your models on a particular font being available.
*/
/*!
  \var SoSFFloat SoFont::size

  Size of font. Defaults to 10.0.

  For 2D rendered bitmap fonts (like for SoText2), this value is the
  height of a character in screen pixels. For 3D text, this value is
  the world-space coordinates height of a character in the current
  units setting (see documentation for SoUnits node).
*/

// *************************************************************************

SO_NODE_SOURCE(SoFont);

/*!
  Constructor.
*/
SoFont::SoFont(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoFont);

  SO_NODE_ADD_FIELD(name, ("defaultFont"));
  SO_NODE_ADD_FIELD(size, (10.0f));
}

/*!
  Destructor.
*/
SoFont::~SoFont()
{
}

// Doc from superclass.
void
SoFont::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoFont, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoCallbackAction, SoFontNameElement);
  SO_ENABLE(SoCallbackAction, SoFontSizeElement);
  SO_ENABLE(SoGLRenderAction, SoFontNameElement);
  SO_ENABLE(SoGLRenderAction, SoFontSizeElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoFontNameElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoFontSizeElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoFontNameElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoFontSizeElement);
  SO_ENABLE(SoPickAction, SoFontNameElement);
  SO_ENABLE(SoPickAction, SoFontSizeElement);
}

// Doc from superclass.
void
SoFont::doAction(SoAction * action)
{
  SoState * state = action->getState();
  uint32_t flags = SoOverrideElement::getFlags(state);

#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)

  if (!name.isIgnored() && !TEST_OVERRIDE(FONT_NAME)) {
    SoFontNameElement::set(state, this, this->name.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setFontNameOverride(state, this, TRUE);
    }
  }
  if (!size.isIgnored() && !TEST_OVERRIDE(FONT_SIZE)) {
    SoFontSizeElement::set(state, this, this->size.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setFontSizeOverride(state, this, TRUE);
    }
  }

#undef TEST_OVERRIDE
}

// Doc from superclass.
void
SoFont::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoFont::doAction(action);
}

// Doc from superclass.
void
SoFont::GLRender(SoGLRenderAction * action)
{
  SoFont::doAction(action);
}

// Doc from superclass.
void
SoFont::callback(SoCallbackAction * action)
{
  SoFont::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoFont::pick(SoPickAction * action)
{
  SoFont::doAction(action);
}

// Doc from superclass.
void
SoFont::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoFont::doAction(action);
}
